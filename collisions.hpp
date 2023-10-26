#pragma once
#include <cmath>

#include "flatVector.hpp"
#include "flatMath.hpp"

namespace collisions
{
	//project a set of point verticies of one shape onto a vector, return max and min
	void projectVerticies(std::vector<FlatVector> vertices, FlatVector axis, float* min, float* max)
	{
		*max = std::numeric_limits<float>::min();
		*min = std::numeric_limits<float>::max();

		for (int i = 0; i < vertices.size(); i++)
		{
			float projection = flatMath::dotProd(vertices[i], axis);

			if (projection < *min) { *min = projection; }
			else if (projection > *max) { *max = projection; }
		}

	}

	//using SAT
	bool intersectPolygons(std::vector<FlatVector> verticesA, std::vector<FlatVector> verticesB)
	{
		//loop through first shape
		for (int i = 0; i < verticesA.size(); i++)
		{
			FlatVector vecA = verticesA[i];
			FlatVector vecB = verticesA[(i + 1) % verticesA.size()];

			//var1 edge of triangle, var2 perpendicular to said edge
			FlatVector edge = vecB - vecA;
			FlatVector axis(-edge.y, edge.x);

			float minA, maxA, minB, maxB;
			projectVerticies(verticesA, axis, &minA, &maxA);
			projectVerticies(verticesB, axis, &minB, &maxB);

			if (minA >= maxB || maxA <= minB) { return false; }
		}
		//loop through second shape
		for (int i = 0; i < verticesB.size(); i++)
		{
			FlatVector vecA = verticesB[i];
			FlatVector vecB = verticesB[(i + 1) % verticesB.size()];

			//var1 edge of triangle, var2 perpendicular to said edge
			FlatVector edge = vecB - vecA;
			FlatVector axis(-edge.y, edge.x);

			float minA, maxA, minB, maxB;
			projectVerticies(verticesA, axis, &minA, &maxA);
			projectVerticies(verticesB, axis, &minB, &maxB);

			if (minA >= maxB || maxA <= minB) 
			{ return false; }
			
		}
		return true;

	}

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
