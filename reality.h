#pragma once
#include "raylib.h"

class reality {
private:
	Camera2D camera = { 0 };
	Camera3D camera3 = { 0 };
public:
	reality();
	int update();
};
