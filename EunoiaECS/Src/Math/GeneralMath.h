#pragma once
#include <iostream>
#include <cmath>
#include "../Core/Common.h"

// Math contants
#define MATH_PI		3.14159265358979323846
#define MATH_E			2.71828182845904523536
#define MATH_SQRT2		1.41421356237309504880

// Conversion
#define MATH_DEG_TO_RAD(deg) (MATH_PI / 180.0) * deg
#define MATH_RAD_TO_DEG(rad) (180.0 / MATH_PI) * rad

// Min and Max
#define MATH_MIN(n1, n2) n1 <= n2 ? n1 : n2
#define MATH_MAX(n1, n2) n1 >= n2 ? n1 : n2
#define MATH_CLAMP(x, upper, lower) MATH_MIN(upper, MATH_MAX(lower, x))

#define MATH_LERP(a, b, c)  a + c * (b - a)

#define MATH_ROTATE_LEFT(num, distance) Eunoia::Math::math_rotate_left(num, distance)
#define MATH_REVERSE_INT(num) Eunoia::Math::math_reverse_int(num)

namespace Eunoia { namespace Math {

	static int math_reverse_int(int num)
	{
		unsigned int i = num;

		i = (i & 0x55555555) << 1 | (i >> 1) & 0x55555555;
		i = (i & 0x33333333) << 2 | (i >> 2) & 0x33333333;
		i = (i & 0x0f0f0f0f) << 4 | (i >> 4) & 0x0f0f0f0f;
		i = (i << 24) | ((i & 0xff00) << 8) |
			((i >> 8) & 0xff00) | (i >> 24);

		return i;
	}

	static int math_rotate_left(int num, int distance)
	{
		unsigned int i = num;

		i = (i << distance) | (i >> -distance);

		return i;
	}

} }
