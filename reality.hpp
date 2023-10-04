#pragma once
#include "flatVector.hpp"
#include "flatMath.hpp"
#include "flatBody.hpp"

#include "raylib.h"
#include <vector>
#include <cstdlib>

class reality {
private:
	Camera2D camera = { 0 };
	Camera3D camera3 = { 0 };

	//creating required vectors
	FlatVector* testVec = new FlatVector(0, 0);

	//creating required bodies
	std::vector<flatBody*> bodies;
	flatBody* testBod = createCircleBody(5, FlatVector(0, 0), 5, 1, 1);
	
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
		int bodyCount = 20;
		for (int i=0; i < bodyCount; i+=1)
		{
			int randWidth = -GetScreenWidth() / 2 + (rand() % static_cast<int>(GetScreenWidth() / 2
				- -GetScreenWidth() / 2 + 1));

			int randHeight = -GetScreenHeight() / 2 + (rand() % static_cast<int>(GetScreenHeight() / 2
				- -GetScreenHeight() / 2 + 1));
			
			bodies.push_back(createCircleBody(5, FlatVector(randWidth, randHeight), 5, 1, 1));
		}
	}

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
		float speed = 10;
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
		
		//start drawing
		BeginDrawing();
		
		//set constant screen settings
		ClearBackground(LIGHTGRAY);
		DrawFPS(0, 0);
		BeginMode2D(camera);
		DrawGrid(100, 20);
		
		DrawCircle(0, 0, testVec->length(*testVec), BLUE);
		for (int i = 0; i < bodies.size(); i += 1)
		{
			DrawCircle(bodies[i]->position.x, bodies[i]->position.y, 50, BLUE);
		}
		
		DrawCircle(testBod->position.x, testBod->position.y, 5, BLUE);
		DrawLine(0, 0, testVec->x, testVec->y, GREEN);

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

