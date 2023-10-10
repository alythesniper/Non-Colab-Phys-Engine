#pragma once
#include "flatVector.hpp"
#include "flatMath.hpp"

namespace collisions
{
	bool intersectCircles(FlatVector centerA, float radiusA, FlatVector centerB,
		float radiusB, FlatVector* normal, float* depth)
	{
		float distance = flatMath::distance(centerA, centerB);
		float radii = radiusA + radiusB;

		if (distance >= radii)
		{
			return false;
		}

		*normal = *flatMath::normalise(centerB - centerA);
		*depth = radii - distance;
		return true;
	}
}
