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
		bodies.push_back(createBoxBody(50, 50, { 0,0 }, 5, 1, 1));
		bodies.push_back(createBoxBody(50, 50, { 80,70 }, 5, 1, 1));
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
		
		for (int i = 0; i < bodies.size(); i++)
		{
			bodies[i]->rotate(PI / 680);
			float radius = 0.5 * sqrt(pow(bodies[i]->width, 2) + pow(bodies[i]->height, 2));
			DrawPoly(bodies[i]->getPosition().toVector2(), 4, radius, (bodies[i]->getRotation()*(180/PI))+45, BLUE);
			//check for collisions
			for (int body2 = i+1; body2 < bodies.size(); body2++)
			{
				if (collisions::intersectPolygons(bodies[i]->getTransformedVertices(), bodies[body2]->getTransformedVertices()))
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

