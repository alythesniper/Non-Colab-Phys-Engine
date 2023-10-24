#pragma once
#include <cmath>

struct flatTransform
{
public:
	float positionx;
	float positiony;
	float Sin;
	float Cos;

	flatTransform(float x, float y, float angle)
	{
		positionx = x;
		positiony = y;
		Sin = sin(angle);
		Cos = cos(angle);
		
	}
};
