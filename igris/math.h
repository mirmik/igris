#ifndef IGRIS_MATH_H
#define IGRIS_MATH_H

#include <igris/math/deg.h>
#include <igris/math/defs.h>

namespace igris 
{
	template<class T>
	T clamp(T c, T a, T b) 
	{
		return c < a ? a : c > b ? b : c;
	}

	template<class T>
	bool inside(T c, T a, T b) 
	{
		return c > a && c < b;	
	}

	template<class T>
	bool outside(T c, T a, T b) 
	{
		return c < a || c > b;	
	}

	using ::deg;
	using ::deg2rad;
	using ::rad2deg;

	template<class A, class B, class C>
	constexpr bool early(A a, B b, C c) 
	{
		return abs(a - b) < c;
	}

	template<class A, class B>
	constexpr bool early(A a, B b) 
	{
		return early(a, b, 1e-6);
	}
} 

#endif