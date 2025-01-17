#pragma once
// Copyright (c) 1997 - 2022 kio@little-bat.de
// BSD-2-Clause license
// https://opensource.org/licenses/BSD-2-Clause


/*	Data type definitions for 2-dimensional geometry

							checked & reworked    2007-06-14 kio !

	Cord	coordinate	= data size of coordinates
	Loc		location 	= coordinates of points
	Dist	distance 	= distance (vector) between points
	Box		rectangle	= rectangular area with top/left point and size

	note: Da es in C++ keine typedefs für Templateklassen gibt,
		  bleibt es bei der Methode mit wählbarem, festem Datentyp Cord.
*/


typedef int	 Cord;		typedef Cord const cCord;
		struct Loc;		typedef Loc const  cLoc;
		struct Dist;	typedef Dist const cDist;
		struct Box;		typedef Box const  cBox;




// ====	offsets, sizes, distances: struct Dist ================================

struct Dist
{
	Cord	dx,dy;

	Dist	( Cord DX=0, Cord DY=0 )	: dx(DX), dy(DY) {}

	Dist&	operator+=	( cDist& d )	{ dx+=d.dx; dy+=d.dy; return *this; }		// Dist + Dist = Dist
	Dist&	operator-=	( cDist& d )	{ dx-=d.dx; dy-=d.dy; return *this; }		// Dist - Dist = Dist
	Dist&	operator/=	( int n )		{ dx/=n;    dy/=n;	  return *this; }
	Dist&	operator*=	( int n )		{ dx*=n;    dy*=n;	  return *this; }

	bool	operator==	( cDist& q ) const	{ return dx==q.dx && dy==q.dy; }
	bool	operator!=	( cDist& q ) const	{ return dx!=q.dx || dy!=q.dy; }
};



// ====	coordinates: struct Loc =========================================

struct Loc
{
	Cord	x,y;

	Loc		( Cord X=0, Cord Y=0 )	: x(X), y(Y) {}

	Loc&	operator+=	( cDist& d )		{ x+=d.dx; y+=d.dy; return *this; }
	Loc&	operator-=	( cDist& d )		{ x-=d.dx; y-=d.dy; return *this; }

	bool	operator==	( cLoc& q ) const	{ return x==q.x && y==q.y; }
	bool	operator!=	( cLoc& q ) const	{ return x!=q.x || y!=q.y; }
};



// ----	inline functions for 'Loc' and 'Dist' -------------------------

inline Loc&   ToLoc( Dist& d )				{ return reinterpret_cast<Loc&>(d); }	// explicit cast
inline cLoc&  ToLoc( cDist& d )				{ return reinterpret_cast<cLoc&>(d); }	// explicit cast
inline Dist&  ToDist( Loc& p )				{ return reinterpret_cast<Dist&>(p); }	// explicit cast
inline cDist& ToDist( cLoc& p )				{ return reinterpret_cast<cDist&>(p); }	// explicit cast


inline double Ascent( cDist& d )			{ return double(d.dx)    / double(d.dy);    }
inline double Ascent( cLoc& a, cLoc& e )	{ return double(e.x-a.x) / double(e.y-a.y); }


inline Dist	operator+ ( Dist d, cDist& q )	{ return d+=q; }				// Dist + Dist = Dist
inline Dist	operator- ( Dist d, cDist& q )	{ return d-=q; }				// Dist - Dist = Dist
inline Dist	operator/ ( Dist d, int n )		{ return d/=n; }
inline Dist	operator* ( Dist d, int n )		{ return d*=n; }
inline Dist	operator- ( cDist& d )			{ return Dist(-d.dx,-d.dy); }		// unärer operator -
inline Dist	operator- ( cLoc& p, cLoc& q )	{ return Dist(p.x-q.x,p.y-q.y); }	// Loc - Loc = Dist


inline Loc	operator+ ( Loc p, cDist& d )	{ return p+=d; }				// Loc + Dist = Loc
inline Loc	operator- ( Loc p, cDist& d )	{ return p-=d; }				// Loc - Dist = Loc



// ====	rectangles  [p1 ... [p2 ======================================

/*	Empty Box  <=>  width <= 0  ||  height <= 0

	most class Box member functions set  width = height = 0  if returning an empty box.
	but this is not strictly enforced.
*/

struct Box
{
	Loc		p1;			// top left corner
	Dist	sz;			// width and height

// simplify access to  Cords:
// unions would be even better but disallow ctors for Loc and Dist!
	#define x1	p1.x
	#define	y1	p1.y
	#define	w	sz.dx
	#define	h	sz.dy


// accessing the members
	cLoc&	P1			( )	const noexcept		{ return p1; }
	Loc		P2			( ) const noexcept		{ return p1+sz; }
	Cord	X1			( )	const noexcept		{ return x1; }
	Cord	Y1			( )	const noexcept		{ return y1; }
	Cord	X2			( )	const noexcept		{ return x1+w; }
	Cord	Y2			( )	const noexcept		{ return y1+h; }

	Cord 	Width		( ) const noexcept		{ return w; }
	Cord	Height		( ) const noexcept		{ return h; }
	cDist&	Size		( ) const noexcept		{ return sz; }
	Cord	HCenter		( )	const noexcept		{ return x1+w/2; }
	Cord	VCenter		( )	const noexcept		{ return y1+h/2; }
	Loc		Center		( )	const noexcept		{ return Loc(HCenter(),VCenter()); }

// set position and size:
	void	SetX1		( Cord X ) noexcept		{ x1 = X; }
	void	SetY1		( Cord Y ) noexcept		{ y1 = Y; }
	void	SetWidth	( Cord W ) noexcept		{ w = W; }
	void	SetHeight	( Cord H ) noexcept		{ h = H; }
	void	SetX2		( Cord X ) noexcept		{ w = X-x1; }
	void	SetY2		( Cord Y ) noexcept		{ h = Y-y1; }

	void	SetP1		( Cord X, Cord Y ) noexcept	{ x1 = X;   y1 = Y; }
	void	SetSize		( Cord W, Cord H ) noexcept	{ w = W;    h = H; }
	void	SetP2		( Cord X, Cord Y ) noexcept	{ w = X-x1; h = Y-y1; }

	void	SetP1		( cLoc& P ) noexcept	{ p1 = P; }
	void	SetSize		( cDist& SZ ) noexcept	{ sz = SZ; }
	void	SetP2		( cLoc& P ) noexcept	{ w = P.x-x1; h = P.y-y1; }

// original creators:
	Box		( )									: p1(0,0), sz(0,0) {}
	Box		( cLoc& P1, cLoc& P2 )				: p1(P1),  sz(P2-P1) {}
	Box		( cLoc& P1, cDist& SZ )				: p1(P1),  sz(SZ) {}
	Box		( cDist& SZ )						: p1(0,0), sz(SZ) {}
	Box		( Cord X, Cord Y, Cord W, Cord H )	: p1(X,Y), sz(W,H) {}	// caveat: not Box(x1,y1,x2,y2)
	Box		( Cord W, Cord H )					: p1(0,0), sz(W,H) {}

// compare:
bool	 	operator==	( cBox& q ) const		{ return (p1==q.p1) && (sz==q.sz); }
bool		operator!=	( cBox& q ) const		{ return (p1!=q.p1) || (sz!=q.sz); }

// geometrical operators:
Box&		operator+=	( cBox& );				// Enclosing rectangle
Box&		operator-= 	( cBox& );				// Punch
Box& 		operator^= 	( cBox& );				// Intersection

// grow / shrink box at all 4 corners
Box&		Grow		( Cord d )				{ x1-=d;  y1-=d;  w+=2*d;  h+=2*d;  return *this; }
Box&		Grow		( Cord dx, Cord dy )	{ x1-=dx; y1-=dy; w+=2*dx; h+=2*dy; return *this; }
Box&		Grow		( cDist& d )			{ return Grow(d.dx,d.dy); }

// grow / shrink box by moving bottom/right corner
Box&		Resize		( cDist& SZ )			{ sz = SZ; return *this; }
Box& 		Resize		( Cord W, Cord H )		{ w = W; h = H; return *this; }

// move box
Box&		Move 		( Cord dx, Cord dy )	{ x1 += dx; y1 += dy;  return *this; }
Box&		Move 		( cDist& d )			{ p1 += d; return *this; }
Box&		operator+=	( cDist& d )			{ return Move(d); }
Box&		operator-=	( cDist& d )			{ return Move(-d.dx,-d.dy); }

// test for enclosure and intersection:
bool		Encloses	( Cord X, Cord Y ) const{ return X>=x1 && X<x1+w && Y>=y1 && Y<y1+h; }	// location
bool		Encloses	( cLoc& p )	const		{ return Encloses( p.x, p.y ); }				// location
bool		Encloses	( cBox& i )	const		{ return i.x1>=x1&&i.y1>=y1&&i.x1+i.w<=x1+w&&i.y1+i.h<=y1+h; }	// entire box
bool		Intersect 	( cBox& q ) const		{ return x1<q.x1+q.w&&q.x1<x1+w&&y1<q.y1+q.h&&q.y1<y1+h; }

bool		IsEmpty		( ) const				{ return w<=0 || h<=0; }
bool		IsNotEmpty	( ) const				{ return w>0  && h>0;  }
};


inline Box	operator+	( Box q, cDist& d )		{ return q+=d; }		// shift box
inline Box	operator-	( Box q, cDist& d )		{ return q-=d; }		// shift box
inline Box	operator+	( Box q, cBox& b2 )		{ return q+=b2; }		// enclosing rectangle
inline Box	operator-	( Box q, cBox& b2 )		{ return q-=b2; }		// punch
inline Box	operator^	( Box q, cBox& b2 )		{ return q^=b2; }		// intersection


/*	calculate box on alert position inside other box
*/
inline Box AlertBox ( cBox& bbox, cDist& size )
{
	return Box ( Loc( bbox.x1+(bbox.w-size.dx)/2, bbox.y1+20+(bbox.h-size.dy-20)/3 ), size );
}


#undef x1
#undef y1
#undef w
#undef h

















