#pragma once
#include "flatVector.hpp"
#include "flatMath.hpp"
#include "flatBody.hpp"
#include "collisions.hpp"

#include "raylib.h"
#include <vector>

class reality {
private:
	Camera2D camera = { 0 };
	Camera3D camera3 = { 0 };

	//creating required bodies
	std::vector<flatBody*> bodies;
	
	//vertex array
	std::vector<Vector2> vertexBuffer;
public:
	//using SAT
	bool intersectPolygonst(std::vector<FlatVector> verticesA, std::vector<FlatVector> verticesB)
	{
		//loop through first shape
		for (int i = 0; i < verticesA.size(); i++)
		{
			//debugging code
			for (int k = 0; k < verticesB.size(); k++)
			{
				FlatVector vecA = verticesB[k];
				FlatVector vecB = verticesB[(k + 1) % verticesB.size()];

				//var1 edge of polygon, var2 perpendicular to said edge
				FlatVector edge = vecB - vecA;
				FlatVector axis(-edge.y, edge.x); 
				
				DrawLine(verticesB[k].x, verticesB[k].y, verticesB[k].x + edge.x, verticesB[k].y + edge.y, BLUE);
				printf("edge shapeB: %f, %f\n", edge.x, edge.y);
				//draw normals of sides
				DrawLine((verticesB[k]+(edge/2)).x, (verticesB[k] + (edge / 2)).y, (verticesB[k] + (edge / 2)).x + axis.x,
				(verticesB[k] + (edge / 2)).y + axis.y, BLUE);
			}
			for (int m = 0; m < verticesB.size(); m++)
			{
				FlatVector vecA = verticesA[m];
				FlatVector vecB = verticesA[(m + 1) % verticesA.size()];

				//var1 edge of polygon, var2 perpendicular to said edge
				FlatVector edge = vecB - vecA;
				FlatVector axis(-edge.y, edge.x);

				DrawLine(verticesA[m].x, verticesA[m].y, verticesA[m].x + edge.x, verticesA[m].y + edge.y, RED);
				//print edge
				printf("edge shapeA: %f, %f\n", edge.x, edge.y);
				DrawLine((verticesA[m]+(edge/2)).x, (verticesA[m] + (edge / 2)).y, (verticesA[m] + (edge / 2)).x + axis.x,
				(verticesA[m] + (edge / 2)).y + axis.y, RED);
			}
			
			FlatVector vecA = verticesA[i];
			FlatVector vecB = verticesA[(i + 1) % verticesA.size()];

			//var1 edge of polygon, var2 perpendicular to said edge
			FlatVector edge = vecB - vecA;
			FlatVector axis(-edge.y, edge.x);

			
			float maxA = std::numeric_limits<float>::min();
			float maxB = std::numeric_limits<float>::min();
			float minA = std::numeric_limits<float>::max();
			float minB = std::numeric_limits<float>::max();

			//project verticies onto axis
			for (int i = 0; i < verticesA.size(); i++)
			{
				float projection = flatMath::dotProd(verticesA[i], axis);

				if (projection < minA) { minA = projection; }
				else if (projection > maxA) { maxA = projection; }
			}
			for (int i = 0; i < verticesB.size(); i++)
			{
				float projection = flatMath::dotProd(verticesB[i], axis);

				if (projection < minB) { minB = projection; }
				else if (projection > maxB) { maxB = projection; }
			}
			if (minA >= maxB || maxA <= minB)
			{
			
				return false;
			}

		}
		//loop through second shape
		for (int i = 0; i < verticesB.size(); i++)
		{
			FlatVector vecA = verticesB[i];
			FlatVector vecB = verticesB[(i + 1) % verticesB.size()];

			//var1 edge of polygon, var2 perpendicular to said edge
			FlatVector edge = vecB - vecA;
			FlatVector axis(-edge.y, edge.x);

			float maxA = std::numeric_limits<float>::min();
			float maxB = std::numeric_limits<float>::min();
			float minA = std::numeric_limits<float>::max();
			float minB = std::numeric_limits<float>::max();
			//project verticies onto axis
			for (int i = 0; i < verticesA.size(); i++)
			{
				float projection = flatMath::dotProd(verticesA[i], axis);

				if (projection < minA) { minA = projection; }
				else if (projection > maxA) { maxA = projection; }
			}
			for (int i = 0; i < verticesB.size(); i++)
			{
				float projection = flatMath::dotProd(verticesB[i], axis);

				if (projection < minB) { minB = projection; }
				else if (projection > maxB) { maxB = projection; }
			}
			if (minA >= maxB || maxA <= minB)
			{
				return false;
			}

		}
		return true;

	}
	reality()
	{
		//create window and enable VSYNC
		InitWindow(1700, 850, "Physics Engine");
		SetWindowState(FLAG_VSYNC_HINT);
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		camera.zoom = 1.0f;
		camera.target = { 0.0f, 0.0f };

		//create required bodies
		/*
		int bodyCount = 2;
		for (int i=0; i < bodyCount; i+=1)
		{
			int randWidth = -GetScreenWidth() / 2 + (rand() % static_cast<int>(GetScreenWidth() / 2
				- -GetScreenWidth() / 2 + 1));

			int randHeight = -GetScreenHeight() / 2 + (rand() % static_cast<int>(GetScreenHeight() / 2
				- -GetScreenHeight() / 2 + 1));
			
			bodies.push_back(createBoxBody(20, 20, {0,0}, 5, 1, 1));
			bodies[i]->move({ (float)randWidth, (float)randHeight });
		}
		*/
		bodies.push_back(createBoxBody(70, 70, { 200,200 }, 5, 1, 1));
		bodies.push_back(createBoxBody(70, 70, { 80,70 }, 5, 1, 1));

		
	}
	reality(bool isApi)
	{
		reality();
		if (isApi == true)
		{
		}
	}

	//update function, runs every frame
	int update()
	{
		
		//get current screen parameters
		int screenWidth = GetScreenWidth();
		int screenHeight = GetScreenHeight();

		//move screen origin to center of window
		camera.offset = { (float)screenWidth/2, (float)screenHeight/2 };
		
		//camera movement controls
		if (IsKeyDown(KEY_RIGHT))
		{
			camera.rotation++;
		}
		if (IsKeyDown(KEY_LEFT))
		{
			camera.rotation--;
		}
		//camera zoom controls
		camera.zoom += ((float)GetMouseWheelMove() * 0.5f);

		//object movement
		float dx = 0;
		float dy = 0;
		float speed = 5;
		
		if (IsKeyDown(KEY_A)) { dx--; }
		if (IsKeyDown(KEY_D)) { dx++; }
		//for some reason these have to be opposite
		if (IsKeyDown(KEY_S)) { dy++; }
		if (IsKeyDown(KEY_W)) { dy--; }

		if (dx != 0 || dy != 0)
		{
			FlatVector direction = *flatMath::normalise(*(new FlatVector(dx, dy)));
			FlatVector velocity = direction * speed;
			bodies[0]->move(velocity);
		}
/*
		//circle collision reaction code
		for (int i = 0; i < bodies.size()-1; i++)
		{
			flatBody* bodyA = bodies[i];
			
			for (int j = i+1; j < bodies.size(); j++)
			{
				
				flatBody* bodyB = bodies[j];
				FlatVector* normal = new FlatVector(0, 0);
				float depth = 0;
				if (collisions::intersectCircles(bodies[i]->getPosition(), bodyA->radius,
					bodies[j]->getPosition(), bodyB->radius, normal, &depth))
				{
					bodyA->move(-*normal * depth * 1 / 2);
					bodyB->move(*normal * depth * 1 / 2);
				}
				
			}
		}
*/

		//start drawing
		BeginDrawing();
		
		//set constant screen settings
		ClearBackground(LIGHTGRAY);
		DrawFPS(0, 0);
		BeginMode2D(camera);
		DrawLine(-1000, 0, 1000, 0, GREEN);
		DrawLine(0, -1000, 0, 1000, GREEN);
		
		for (int i = 0; i < bodies.size(); i++)
		{
			bodies[i]->rotate(PI / 680);
			float radius = 0.5 * sqrt(pow(bodies[i]->width, 2) + pow(bodies[i]->height, 2));
			DrawPoly(bodies[i]->getPosition().toVector2(), 4, radius, (bodies[i]->getRotation()*(180/PI))+45, BLUE);
			//draw circles on each vertex
			for(int j = 0; j < bodies[i]->getTransformedVertices().size(); j++)
			{
				DrawCircle(bodies[i]->getTransformedVertices()[j].x, bodies[i]->getTransformedVertices()[j].y, 1, RED);
			}
			//check for collisions
			for (int body2 = i+1; body2 < bodies.size(); body2++)
			{
				if (intersectPolygonst(bodies[i]->getTransformedVertices(), bodies[body2]->getTransformedVertices()))
				{
					DrawText("Collision", 0, 0, 20, RED);
				}
			}
		}
		

		EndMode2D();
		EndDrawing();
		return 0;
	}
		
	//destructor
	~reality()
	{
		CloseWindow();
	}
};

