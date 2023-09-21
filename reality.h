#pragma once
#include "raylib.h"
#include "flatVector.h"
#include "flatMath.h"

class reality {
private:
	Camera2D camera = { 0 };
	Camera3D camera3 = { 0 };
	
	//creating required vectors
	FlatVector* testVec = new FlatVector(0, 0);
public:
	reality();
	int update();
	~reality();
};

