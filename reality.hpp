#pragma once
//removes naming conflicts with windows.h
#define NODRAWTEXT        // DrawText() and DT_*
#define NOUSER            // All USER defines and routines

#include "flatVector.hpp"
#include "flatMath.hpp"
#include "flatBody.hpp"
#include "collisions.hpp"
#include "network.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "network.h"
#include <boost/asio.hpp>
#include <raylib.h>
#include <thread>

class reality {
private:
	Camera2D camera = { 0 };
	Camera3D camera3 = { 0 };

	//creating required bodies
	std::vector<flatBody*> bodies;
	
	//vertex array
	std::vector<Vector2> vertexBuffer;

	boost::asio::io_service io_service1;
	std::thread io_thread;
	std::thread update_thread;
	ArrayReceiver receiver;

public:
	
	//basic body creation functions for cmdline 
	void userCreateBox(float width = 40, float height = 40, FlatVector pos = {-23.3, -23.3}, float density = 5,
	bool isStatic = false, float restitution = 1)
	{
		float randWidth = -GetScreenWidth() / 2 + (rand() % static_cast<int>(GetScreenWidth() / 2
			- -GetScreenWidth() / 2 + 1));

		float randHeight = -GetScreenHeight() / 2 + (rand() % static_cast<int>(GetScreenHeight() / 2
			- -GetScreenHeight() / 2 + 1));

		if (pos.x == -23.3 && pos.y == -23.3)
		{
			pos = { randWidth, randHeight };
		}
		bodies.push_back(createBoxBody(width, height, pos, density, isStatic, restitution));
	}

	//basic body creation functions for cmdline 
	void userCreateCircle(float radius = 20, FlatVector pos = { -23.3, -23.3 }, float density = 5,
		bool isStatic = false, float restitution = 1)
	{
		float randWidth = -GetScreenWidth() / 2 + (rand() % static_cast<int>(GetScreenWidth() / 2
			- -GetScreenWidth() / 2 + 1));

		float randHeight = -GetScreenHeight() / 2 + (rand() % static_cast<int>(GetScreenHeight() / 2
			- -GetScreenHeight() / 2 + 1));

		if (pos.x == -23.3 && pos.y == -23.3)
		{
			pos = { randWidth, randHeight };
		}
		bodies.push_back(createCircleBody(radius, pos, density, isStatic, restitution));
	}


	void clearScreen()
	{
		//clear screen asci codes
		std::cout << "\033[2J\033[1;1H";
	}

	//prints the prompt and takes input
	void inputLoop()
	{

		//get input
		std::cout << "flatphysics> ";
		std::string inputCommand;
		std::cin >> inputCommand;

		//put into lowercase
		std::transform(inputCommand.begin(), inputCommand.end(), inputCommand.begin(),
			[](unsigned char c) { return std::tolower(c); });
		std::istringstream iss(inputCommand);
		std::vector<std::string> tokens;


		if (inputCommand == "createcircle")
		{
			userCreateCircle();
		}
		if (inputCommand == "createbox")
		{
			userCreateBox();
		}
		if (inputCommand == "clear")
		{
			clearScreen();
		}
	}

	reality()
		: receiver(io_service1, 8181)
	{
		//create window and enable VSYNC
		InitWindow(1700, 850, "Physics Engine");
		SetWindowState(FLAG_VSYNC_HINT);
		SetWindowState(FLAG_WINDOW_RESIZABLE);
		camera.zoom = 1.0f;
		camera.target = { 0.0f, 0.0f };

		receiver.start();
		io_thread = std::thread([&]() {
			std::cout << "Starting io_service...\n";
			io_service1.run();
			std::cout << "io_service has finished.\n";
			});

	}

	//update function, runs every frame
	int update()
	{
		if (receiver.recv_count > 0)
		{
			for (int i = 0; i < receiver.big_array_.size(); i++)
			{
				if (receiver.big_array_[i][2] == 1)
				{
					userCreateCircle(receiver.big_array_[i][3], { receiver.big_array_[i][0], receiver.big_array_[i][1] }, 5, false, receiver.big_array_[i][4]);
				}
				else if (receiver.big_array_[i][2] == 2)
				{
					userCreateBox(20, 20, { receiver.big_array_[i][0], receiver.big_array_[i][1] }, 5, false, receiver.big_array_[i][5]);
				}
			}
			receiver.big_array_.clear();
			receiver.recv_count = 0;
		}
		
		//check keys for command option
		if (IsKeyDown(KEY_TAB))
		{
			inputLoop();
		}
		
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
		
		//collision detection loop
		for (int i = 0; i < bodies.size(); i++)
		{
			for (int j = i + 1; j < bodies.size(); j++)
			{
				flatBody* bodyA = bodies[i];
				flatBody* bodyB = bodies[j];
				FlatVector* normal = new FlatVector(0, 0);
				float depth = 0;

				// Circle-Circle collision
				if (bodyA->bodyShapeType == shapeType::circle && bodyB->bodyShapeType == shapeType::circle)
				{
					if (collisions::intersectCircles(bodyA->getPosition(), bodyA->radius, bodyB->getPosition(), bodyB->radius, normal, &depth))
					{
						bodyA->move(-*normal * depth * 1 / 2);
						bodyB->move(*normal * depth * 1 / 2);
					}
				}
				// Box-Box collision
				else if (bodyA->bodyShapeType == shapeType::box && bodyB->bodyShapeType == shapeType::box)
				{
					if (collisions::intersectPolygons(bodyA->getTransformedVertices(), bodyB->getTransformedVertices(), *normal, depth))
					{
						bodyA->move(-*normal * depth * 1 / 2);
						bodyB->move(*normal * depth * 1 / 2);
					}
				}
				// Circle-Box collision
				else if (bodyA->bodyShapeType == shapeType::circle && bodyB->bodyShapeType == shapeType::box)
				{
					if (collisions::intersectCirclePolygon(bodyA->getPosition(), bodyA->radius, bodyB->getTransformedVertices(), *normal, depth))
					{
						bodyA->move(-*normal * depth * 1 / 2);
						bodyB->move(*normal * depth * 1 / 2);
					}
				}
				else if (bodyA->bodyShapeType == shapeType::box && bodyB->bodyShapeType == shapeType::circle)
				{
					if (collisions::intersectCirclePolygon(bodyB->getPosition(), bodyB->radius, bodyA->getTransformedVertices(), *normal, depth))
					{
						bodyB->move(-*normal * depth * 1 / 2);
						bodyA->move(*normal * depth * 1 / 2);
					}
				}
			}
		}

		//start drawing
		BeginDrawing();
		
		//set constant screen settings
		ClearBackground(BLACK);
		DrawFPS(0, 0);
		BeginMode2D(camera);
		//drawing x,y grid
		DrawLine(-screenWidth, 0, screenHeight, 0, LIGHTGRAY);
		DrawLine(-screenWidth, 1, screenHeight, 1, LIGHTGRAY);
		DrawLine(-screenWidth, 2, screenHeight, 2, LIGHTGRAY);
		
		DrawLine(0, -screenWidth, 0, screenHeight, LIGHTGRAY);
		DrawLine(1, -screenWidth, 1, screenHeight, LIGHTGRAY);
		DrawLine(2, -screenWidth, 2, screenHeight, LIGHTGRAY);
		
		for (int i = -screenWidth; i < screenHeight; i += 50)
		{
			DrawLine(i, -screenWidth, i, screenHeight, LIGHTGRAY);
			DrawLine(-screenWidth, i, screenHeight, i, LIGHTGRAY);
		}
		
		
		//Shape Drawing Loop
		for (int i = 0; i < bodies.size(); i++)
		{
			bodies[i]->rotate(PI / 680);
			float boxRadius = 0.5 * sqrt(pow(bodies[i]->width, 2) + pow(bodies[i]->height, 2));
			if (bodies[i]->bodyShapeType == shapeType::box)
			{
				DrawPoly(bodies[i]->getPosition().toVector2(), 4, boxRadius, (bodies[i]->getRotation() * (180 / PI)) + 45, BLUE);
			}
			else if (bodies[i]->bodyShapeType == shapeType::circle)
			{
				DrawCircle(bodies[i]->getPosition().x, bodies[i]->getPosition().y, bodies[i]->radius, YELLOW);
			}
			//check for collisions
			FlatVector* normal = new FlatVector(0, 0);
			float depth = 0;
			for (int body2 = i + 1; body2 < bodies.size(); body2++)
			{
				bool collision = false;
				if (bodies[i]->bodyShapeType == shapeType::circle && bodies[body2]->bodyShapeType == shapeType::circle)
				{
					collision = collisions::intersectCircles(bodies[i]->getPosition(), bodies[i]->radius, bodies[body2]->getPosition(), bodies[body2]->radius, normal, &depth);
				}
				else if (bodies[i]->bodyShapeType == shapeType::box && bodies[body2]->bodyShapeType == shapeType::box)
				{
					collision = collisions::intersectPolygons(bodies[i]->getTransformedVertices(), bodies[body2]->getTransformedVertices(), *normal, depth);
				}
				else if (bodies[i]->bodyShapeType == shapeType::circle && bodies[body2]->bodyShapeType == shapeType::box)
				{
					collision = collisions::intersectCirclePolygon(bodies[i]->getPosition(), bodies[i]->radius, bodies[body2]->getTransformedVertices(), *normal, depth);
				}
				else if (bodies[i]->bodyShapeType == shapeType::box && bodies[body2]->bodyShapeType == shapeType::circle)
				{
					collision = collisions::intersectCirclePolygon(bodies[body2]->getPosition(), bodies[body2]->radius, bodies[i]->getTransformedVertices(), *normal, depth);
				}
				if (collision)
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
		io_service1.stop();
		io_thread.join();
		CloseWindow();
	}
};
