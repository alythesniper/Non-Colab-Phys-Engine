#include "flatVector.h"

const struct FlatVector
{
	float x, y;

	FlatVector(float X, float Y)
	{
		x = X;
		y = Y;
	}

	FlatVector* operator + (FlatVector& b)
	{
		return new FlatVector(x + b.x, y + b.y);
	}
};