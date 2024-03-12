#pragma once
//removes naming conflicts with windows.h
#define NODRAWTEXT        //DrawText() and DT_*
#define NOUSER            //All USER defines and routines

#include "flatVector.hpp"
#include "flatMath.hpp"
#include "flatBody.cpp"
#include "collisions.hpp"
#include "network.h"
#include "flatWorld.hpp"

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

	FlatWorld* world = new FlatWorld;
	int getNum = 0;
	
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
		world->addBody(createBoxBody(width, height, pos, density, isStatic, restitution));
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
		world->addBody(createCircleBody(radius, pos, density, isStatic, restitution));
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


		//create circle command
		if (inputCommand == "createcircle")
		{
			userCreateCircle();
		}
		//create box command
		if (inputCommand == "createbox")
		{
			userCreateBox();
		}
		//clear comman
		if (inputCommand == "clear")
		{
			clearScreen();
		}
		//exit command
		if (inputCommand == "exit")
		{
			exit(0);
		}
		//help command
		if (inputCommand == "help")
		{
			std::cout << "Commands:\n";
			std::cout << "createcircle - creates a circle\n";
			std::cout << "createbox - creates a box\n";
			std::cout << "clear - clears the screen\n";
			std::cout << "exit - exits the program\n";
			std::cout << "help - displays this message\n";
			//keybinds
			std::cout << "Keybinds:\n";
			std::cout << "WASD - moves the object\n";
			std::cout << "J - switches objects\n";
			std::cout << "Left and Right keys - rotates the camera\n";
			std::cout << "Mouse Wheel - zooms in and out\n";

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
			//std::cout << "Starting io_service...\n";
			io_service1.run();
			//std::cout << "io_service has finished.\n";
			});

		world->addBody(createBoxBody(50, 50, { 0, 0 }, 5, true, 1));
		world->addBody(createCircleBody(50, { 100, 105 }, 5, 5, 5));
		
		std::cout << "\033[2J\033[1;1H";
		std::cout << "Welcome to FlatPhysics V1.0! Press 'Tab' to type a command, or type 'help' for a list of commands.\n";
		std::cout << "Visit https://localhost:5000 to view the web interface.\n";

	}

	//update function, runs every frame
	int update(float time)
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
		float forceMagniute = 10;
		
		if (IsKeyDown(KEY_A)) { dx--; }
		if (IsKeyDown(KEY_D)) { dx++; }
		//The y axis is inverted
		if (IsKeyDown(KEY_S)) { dy++; }
		if (IsKeyDown(KEY_W)) { dy--; }
		if (IsKeyPressed(KEY_J))
		{
			getNum++;
			if (getNum > world->bodies.size()-1)
			{
				getNum = 0;
			}
		}

		flatBody* movBody = world->getBody(getNum);

		if (dx != 0 || dy != 0)
		{
			FlatVector forceDirection = *flatMath::normalise(*(new FlatVector(dx, dy)));
			FlatVector force = forceDirection * forceMagniute;
			movBody->addForce(force);
		}
		
		world->Step(time);
		
		//start drawing
		BeginDrawing();
		
		//set constant screen settings
		ClearBackground(BLACK);
		DrawFPS(0, 0);
		BeginMode2D(camera);
		
		//drawing x,y grid
		Vector2 start = { -std::numeric_limits<int>::max(), 0};
		Vector2 end = { std::numeric_limits<int>::max(), 0};
		DrawLineEx(start, end, 4, LIGHTGRAY);
		
		Vector2 startY = { 0, std::numeric_limits<int>::max()};
		Vector2 endY = { 0, -std::numeric_limits<int>::max()};
		DrawLineEx(startY, endY, 4, LIGHTGRAY);
		
		for (int i = -screenWidth-1000; i < screenHeight+1000; i += 50)
		{
			DrawLine(i, -std::numeric_limits<int>::max(), i, std::numeric_limits<int>::max(), LIGHTGRAY);
			DrawLine(-std::numeric_limits<int>::max(), i, std::numeric_limits<int>::max(), i, LIGHTGRAY);
		}
		
		
		//Shape Drawing Loop
		for (int i = 0; i < world->getbodiesCount(); i++)
		{
			flatBody* curBody = world->getBody(i);
			float boxRadius = 0.5 * sqrt(pow(curBody->width, 2) + pow(curBody->height, 2));
			if (curBody->bodyShapeType == shapeType::box)
			{
				DrawPoly(curBody->getPosition().toVector2(), 4, boxRadius, (curBody->getRotation() * (180 / PI)) + 45, BLUE);
			}
			else if (curBody->bodyShapeType == shapeType::circle)
			{
				DrawCircle(curBody->getPosition().x, curBody->getPosition().y, curBody->radius, YELLOW);
			}
			//check for collisions
			FlatVector* normal = new FlatVector(0, 0);
			float depth = 0;
			for (int j = 0; j < world->getbodiesCount(); j++)
			{
				if (i !=j && world->collide(curBody, world->getBody(j), *normal, depth))
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
