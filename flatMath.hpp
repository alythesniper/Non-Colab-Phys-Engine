#pragma once
#include "flatVector.hpp"
#include <cmath>

namespace flatMath
{
	float clamp(float value, float min, float max)
	{
		if (min == max)
		{
			return min;
		}
		if (min > max)
		{
			//do error logging
			throw _EMPTY_ARGUMENT;
		}
		if (value < min)
		{
			return min;
		}
		if (value > max)
		{
			return max;
		}
	}
	float length(FlatVector vec1)
	{
		return sqrt(pow(vec1.x, 2) + pow(vec1.y, 2));
	}
	float distance(FlatVector vec1, FlatVector vec2)
	{
		//return sqrt(pow(sqrt(vec1.x * vec1.x + vec1.y * vec1.y - sqrt(vec1.x * vec1.x + vec1.y * vec1.y)), 2));
		return sqrt(pow((vec1.x - vec2.x), 2) + pow((vec1.y - vec2.y), 2));
	}
	FlatVector* normalise(FlatVector vec1)
	{
		float len = length(vec1);
		return new FlatVector(vec1.x / len, vec1.y / len);
	}
	float dotProd(FlatVector vec1, FlatVector vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}
	float crossProd(FlatVector vec1, FlatVector vec2)
	{
		return vec1.x * vec2.y - vec1.y * vec2.x;
	}
};

