// Copyright (c) 2015 - 2021 kio@little-bat.de
// BSD-2-Clause license
// https://opensource.org/licenses/BSD-2-Clause


#include "kio/kio.h"
#include "TestTimer.h"
#include "cstrings/cstrings.h"


TestTimer::TestTimer()
{
	_start = now();
}


void TestTimer::start()
{
	_start = now();
}


void TestTimer::test(double maxdelay, cstr sourceposition)
{
	double end   = now();
	double delay = end-_start;
	_start = end;

	if(delay > maxdelay)
	{
		if (maxdelay >= 1.0) logline("TT: %s took %.3f sec", sourceposition, delay);
		else if (maxdelay >= 1e-3) logline("TT: %s took %.3f msec", sourceposition, delay*1e3);
		else logline("TT: %s took %.3f µsec", sourceposition, delay*1e6);
	}
}


