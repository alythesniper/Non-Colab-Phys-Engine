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
	
	FlatVector* findAritmeticMean(std::vector<FlatVector> vertices)
	{
		float sumX = 0;
		float sumY = 0;

		for (int i = 0; i < vertices.size(); i++)
		{
			FlatVector vertex = vertices[i];
			sumX += vertices[i].x;
			sumY += vertices[i].y;
		}

		return new FlatVector(sumX / vertices.size(), sumY / vertices.size());
	}

	
	void projectCircle(FlatVector center, float radius, FlatVector axis, float* min, float* max)
	{
		FlatVector direction = *flatMath::normalise(axis);
		FlatVector directionRadius = direction * radius;

		FlatVector pointA = center + directionRadius;
		FlatVector pointB = center - directionRadius;

		*min = flatMath::dotProd(pointA, axis);
		*max = flatMath::dotProd(pointB, axis);

		//swap values if min is greater than max
		if(*min > *max)
		{
			float temp = *min;
			*min = *max;
			*max = temp;
		}
	}

	int closestPointToPoly(FlatVector point, std::vector<FlatVector> vertices)
	{
		float minDistance = std::numeric_limits<float>::max();
		int closestPoint = 0;

		for (int i = 0; i < vertices.size(); i++)
		{
			float distance = flatMath::distance(point, vertices[i]);
			if (distance < minDistance)
			{
				minDistance = distance;
				closestPoint = i;
			}
		}

		return closestPoint;
	}

	bool intersectCirclePolygon(FlatVector centerCircle, float radiusCircle, std::vector<FlatVector> polygonVertices, FlatVector& collisionNormal, float& collisionDepth)
	{
		collisionNormal = { 0, 0 };
		collisionDepth = std::numeric_limits<float>::max();

		FlatVector projectionAxis = { 0, 0 };
		float depthOnAxis = 0.0f;
		float minPolygon, maxPolygon, minCircle, maxCircle;

		// Calculate center of polygon
		FlatVector centerPolygon = { 0, 0 };
		for (const auto& vertex : polygonVertices) {
			centerPolygon += vertex;
		}
		centerPolygon /= static_cast<float>(polygonVertices.size());

		for (int i = 0; i < polygonVertices.size(); i++)
		{
			FlatVector vertexA = polygonVertices[i];
			FlatVector vertexB = polygonVertices[(i + 1) % polygonVertices.size()];

			FlatVector edge = vertexB - vertexA;
			projectionAxis = FlatVector(-edge.y, edge.x);
			projectionAxis = *flatMath::normalise(projectionAxis);

			projectVerticies(polygonVertices, projectionAxis, &minPolygon, &maxPolygon);
			projectCircle(centerCircle, radiusCircle, projectionAxis, &minCircle, &maxCircle);

			if (minPolygon >= maxCircle || minCircle >= maxPolygon)
			{
				return false;
			}

			depthOnAxis = std::min(maxCircle - minPolygon, maxPolygon - minCircle);

			if (depthOnAxis < collisionDepth)
			{
				collisionDepth = depthOnAxis;
				collisionNormal = projectionAxis;
			}
		}

		int closestVertexIndex = closestPointToPoly(centerCircle, polygonVertices);
		FlatVector closestVertex = polygonVertices[closestVertexIndex];

		projectionAxis = closestVertex - centerCircle;
		projectionAxis = *flatMath::normalise(projectionAxis);

		projectVerticies(polygonVertices, projectionAxis, &minPolygon, &maxPolygon);
		projectCircle(centerCircle, radiusCircle, projectionAxis, &minCircle, &maxCircle);

		if (minPolygon >= maxCircle || minCircle >= maxPolygon)
		{
			return false;
		}

		depthOnAxis = std::min(maxCircle - minPolygon, maxPolygon - minCircle);

		if (depthOnAxis < collisionDepth)
		{
			collisionDepth = depthOnAxis;
			collisionNormal = projectionAxis;
		}

		FlatVector direction = centerPolygon - centerCircle;

		if (flatMath::dotProd(direction, collisionNormal) < 0.0f)
		{
			collisionNormal = -collisionNormal;
		}

		return true;
	}

	//using SAT
	bool intersectPolygons(std::vector<FlatVector> verticesA, std::vector<FlatVector> verticesB, FlatVector& normal, float& depth)
	{
		normal = { 0,0 };
		depth = std::numeric_limits<float>::max();
		
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
			else
			{
				//uses min, max instead of 2 comparisons
				float overlap = std::min(maxA, maxB) - std::max(minA, minB);
				if (overlap < depth)
				{
					depth = overlap;
					normal = axis;
				}
			}
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

			if (minA >= maxB || maxA <= minB) { return false; }
			else
			{
				//uses min, max instead of 2 comparisons
				float overlap = std::min(maxA, maxB) - std::max(minA, minB);
				if (overlap < depth)
				{
					depth = overlap;
					normal = axis;
				}
			}
			
		}
		
		depth /= flatMath::length(normal);
		normal = *flatMath::normalise(normal);

		FlatVector* centerA = findAritmeticMean(verticesA);
		FlatVector* centerB = findAritmeticMean(verticesB);
		FlatVector  direction = *flatMath::normalise(*centerB - *centerA);

		if (flatMath::dotProd(normal, direction) < 0)
		{
			normal = -normal;
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
