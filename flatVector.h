#pragma once

const struct FlatVector {
	
	float x, y;

	FlatVector(float X, float Y);
	FlatVector operator + (FlatVector& b);
};
