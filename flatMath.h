#pragma once
#include "flatVector.h"
#include <cmath>

class flatMath
{
public:
	float length(FlatVector vec1);
	float distance(FlatVector vec1, FlatVector vec2);
	FlatVector* normalise(FlatVector vec1);
	float dotProd(FlatVector vec1, FlatVector vec2);
	float crossProd(FlatVector vec1, FlatVector vec2);
};

