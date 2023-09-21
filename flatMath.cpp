#include "flatMath.h"

float FlatVector::length(FlatVector vec1)
{
	return sqrt(pow(vec1.x, 2) + pow(vec1.y, 2));
}

float FlatVector::distance(FlatVector vec1, FlatVector vec2)
{
	return sqrt(pow(sqrt(vec1.x * vec1.x + vec1.y * vec1.y - sqrt(vec1.x * vec1.x + vec1.y * vec1.y)), 2));
}

FlatVector* FlatVector::normalise(FlatVector vec1)
{
	float len = length(vec1);
	return new FlatVector(vec1.x / len, vec1.y / len);
}

float FlatVector::dotProd(FlatVector vec1, FlatVector vec2)
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

float FlatVector::crossProd(FlatVector vec1, FlatVector vec2)
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

