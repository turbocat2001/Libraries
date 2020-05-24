/*	Copyright  (c)	Günter Woigk 2020 - 2020
					mailto:kio@little-bat.de

	This file is free software.

	Permission to use, copy, modify, distribute, and sell this software
	and its documentation for any purpose is hereby granted without fee,
	provided that the above copyright notice appears in all copies and
	that both that copyright notice, this permission notice and the
	following disclaimer appear in supporting documentation.

	THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY,
	NOT EVEN THE IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR
	A PARTICULAR PURPOSE, AND IN NO EVENT SHALL THE COPYRIGHT HOLDER
	BE LIABLE FOR ANY DAMAGES ARISING FROM THE USE OF THIS SOFTWARE,
	TO THE EXTENT PERMITTED BY APPLICABLE LAW.
*/

#pragma once
#include "kio/kio.h"


// Hash functions needed for HashMap


namespace kio
{

	inline uint32 sdbm_hash (const char* key)
	{
		// Calculate hash for key
		//		note: this is the sdbm algorithm:
		//		hash(i) = hash(i-1) * 65599 + str[i];
		//	see: http://www.cse.yorku.ca/~oz/hash.html
		//	https://programmers.stackexchange.com/questions/49550

		uint32 hash = 0;
		while (int c=*key++)
		{
			hash = (hash << 6) + (hash << 16) - hash + uint(c);	// meistens eine Winzigkeit schneller als Multiplikation
		}

		//return hash;					// original algorithm
		return hash ^ (hash>>16);		// do some xor'ing to better support smaller required hash sizes
	}

	inline uint32 hash (const void* key) { return uint32(size_t(key) ^ (size_t(key)>>12)); }

	inline uint32 hash (cstr key)		{ return sdbm_hash(key); }

	inline uint32 hash (uint32 key)		{ return key ^ (key>>16); }
	inline uint32 hash (int32 key)		{ return hash(uint32(key)); }

	inline uint32 hash (uint64 key)		{ return hash(uint32(key) ^ uint32(key>>32)); }
	inline uint32 hash (int64 key)		{ return hash(uint64(key)); }

//	inline uint32 hash (uint key)		{ return key ^ (key>>16); }
//	inline uint32 hash (int key)		{ return hash(uint32(key)); }

#ifndef _LINUX
#if _sizeof_long == 8
	inline uint32 hash (ulong key)		{ return hash(uint64(key)); }
	inline uint32 hash (long key)		{ return hash(uint64(key)); }
#elif _sizeof_long == 4
	inline uint32 hash (ulong key)		{ return hash(uint32(key)); }
	inline uint32 hash (long key)		{ return hash(uint32(key)); }
#else
	#error
#endif
#endif

	// keys must compare what is hashed:
	template<typename T>
	inline bool same (T a, T b) { return a == b; }
	inline bool same (cstr a, cstr b) { return eq(a,b); }
}



