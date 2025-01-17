#pragma once
// Copyright (c) 1995 - 2022 kio@little-bat.de
// BSD-2-Clause license
// https://opensource.org/licenses/BSD-2-Clause

#include "kio/kio.h"


/* ----	Calculate base 2 logarithm or 'position' of leftmost '1' bit -----------------
		2014-02-15: renamed log2 to msbit
		return value:
				msbit(n>0) = int(log(2,n))
		note:	msbit(n=1) = 0
		caveat:	msbit(n=0) = 0		// illegal argument!

		note: template does not work correctly because i don't know how to cast a signed type to a same-sized unsigned type
*/
//INL int msbnibble	(uint8 n) { return (n>7) + (n>3) + (n>1); }										// 0 ..  3
inline int msbit (uint8  n) noexcept { int b=0,i=4; do{ if(n>>i){n>>=i;b+=i;} }while((i>>=1)); return b; } 	// 0 ..  7
inline int msbit (uint16 n) noexcept { int b=0,i=8; do{ if(n>>i){n>>=i;b+=i;} }while((i>>=1)); return b; } 	// 0 .. 15
inline int msbit (uint32 n) noexcept { int b=0,i=16;do{ if(n>>i){n>>=i;b+=i;} }while((i>>=1)); return b; } 	// 0 .. 31
inline int msbit (uint64 n) noexcept { int b=0,i=32;do{ if(n>>i){n>>=i;b+=i;} }while((i>>=1)); return b; } 	// 0 .. 63
inline int msbit (int8  n) noexcept { return msbit(uint8(n));  }
inline int msbit (int16 n) noexcept { return msbit(uint16(n)); }
inline int msbit (int32 n) noexcept { return msbit(uint32(n)); }
inline int msbit (int64 n) noexcept { return msbit(uint64(n)); }


/* ----	Calculate the number of digits required to print a number:
		return value:
				binaryDigits(n=0) = 1
				binaryDigits(n>0) = ceil(msbit(n+1))
*/
template <class T> inline int binaryDigits (T number) noexcept { return msbit(number)  +1; }	// result >= 1
template <class T> inline int hexDigits (T number) noexcept { return msbit(number)/4+1; }	// result >= 1


/* ----	Calculate the number of digits required to store a numbers of a given range:
		used in GifEncoder

		return value:
				reqBits(n) = ceil(msbit(n))
		note:	reqBits(1) = ceil(msbit(1)) = 0
		caveat:	reqBits(0) = ceil(msbit(0)) = 0		// illegal range!
*/
template <class T> inline int reqBits    (T number) noexcept { return number>1 ? msbit(number-1)  +1 : 0; }
template <class T> inline int reqNibbles (T number) noexcept { return number>1 ? msbit(number-1)/4+1 : 0; }
template <class T> inline int reqBytes   (T number) noexcept { return number>1 ? msbit(number-1)/8+1 : 0; }









