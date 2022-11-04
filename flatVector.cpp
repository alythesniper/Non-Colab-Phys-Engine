#include "flatVector.h"

FlatVector::FlatVector(float X, float Y)
	{
		x = X;
		y = Y;
	}

FlatVector FlatVector::operator + (FlatVector& b)
{
	return FlatVector(x + b.x, y + b.y);
}