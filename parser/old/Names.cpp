// Copyright (c) 2018 - 2022 kio@little-bat.de
// BSD-2-Clause license
// https://opensource.org/licenses/BSD-2-Clause

#include "Names.hpp"


/*
	class Names provides uint identifier IDs for strings.
	get(cstr) returns the ID and
	get(uint) returns the string.

	Intended use: for compilers.
	If compiler instances can run in parallel (to compile a bunch of files),
	then each instance must have it's own Names. (else each lookup must be wrapped with a mutex.)
	When linking 2 files, their Names must be merged, which will assign new IDs to the added names.
*/


Names::Names(const cstr* stdnames, uint num_stdnames)
:
	pool(nullptr),
	pool_ptr(nullptr),
	pool_end(nullptr),
	stdnames(stdnames),
	num_stdnames(num_stdnames)
{
	// setup hashmap with standard names
	// => standard names have fixed IDs as enumerated in "idf_id.h"
	// standard names (the keys) are not copied into the mempool
	// names are never removed from hashmap
	// => position in hashmap.items[] / hashmap.keys[] won't change
	// => names for IDs can be looked up with hashmap.keys[ID]
	// IDs for names are looked up with hashmap.get() / hashmap.add()

	for (uint id=0; id<num_stdnames; id++)
	{
		hashmap.add(stdnames[id],IdfID(id));
	}

	assert(hashmap.count() == num_stdnames);	// assert: no idf added twice
}

Names::~Names()
{
	delete pool;
}

void Names::purge()
{
	if (hashmap.count() == num_stdnames) return;
	this->~Names();
	new(this) Names(stdnames,num_stdnames);
}

IdfID Names::add (cstr name)
{
	// add name if not in list
	// return ID

	const uint mapsize = hashmap.count();
	IdfID id = hashmap.get(name,IdfID(mapsize));
	if (id != mapsize) return id;	// name found

	// not found => add to map:

	size_t slen = strlen(name) + 1;
	if (pool_ptr+slen > pool_end)
	{
		assert(slen <= sizeof(pool->data));

		pool = new Chunk(pool);
		pool_ptr = pool->data;
		pool_end = pool->data + sizeof(pool->data);
	}

	memcpy(pool_ptr, name, slen);
	hashmap.add(pool_ptr, id);
	pool_ptr += slen;

	return id;
}

Array<IdfID> Names::merge(const Names& q)
{
	// append names from another Names map.
	// returns array with old-to-new ID mappings.

	Array<IdfID> map(q.hashmap.count());

	for (uint id=0; id<map.count(); id++)
	{
		map[id] = add(q.get(IdfID(id)));
	}

	return map;
}

#define MAGIC 0x2355

void Names::serialize(FD& fd) const
{
	uint a = num_stdnames;
	uint e = hashmap.count();

	fd.write_uint16(MAGIC);
	fd.write_uint32(a);
	fd.write_uint32(e);

	for (uint id=a; id<e; id++)
	{
		fd.write_nstr(get(IdfID(id)));
	}
}

void Names::deserialize(FD& fd)
{
	uint m = fd.read_uint16();
	uint a = fd.read_uint32();
	uint e = fd.read_uint32();

	if (m != MAGIC) throw DataError("Names: wrong MAGIC");
	if (a != num_stdnames) throw DataError("Names: wrong num_stdnames");

	purge();

	for (uint id=a; id<e; id++)
	{
		add(fd.read_nstr());
	}
}



















