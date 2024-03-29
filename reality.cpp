#include "reality.h"

reality::reality() 
{
	//create window and enable VSYNC
	InitWindow(800, 600, "Physics Engine");
    SetWindowState(FLAG_VSYNC_HINT);
	camera.zoom = 1.0f;
	camera.target = { 0.0f, 0.0f};
	
	//move screen origin to center of window
	camera.offset = { 400, 300 };
}

reality::~reality()
{
	CloseWindow();
}


int reality::update()
{
	//camera rotation controls
	if (IsKeyDown(KEY_RIGHT))
	{
		camera.rotation++;
	}
	if (IsKeyDown(KEY_LEFT))
	{
		camera.rotation--;
	}
	
	//camera zoom controls
	camera.zoom += ((float)GetMouseWheelMove()*0.5f);
	
	
	BeginDrawing();
	ClearBackground(BLACK);
	DrawFPS(0, 0);
	BeginMode2D(camera);
	
	//DrawRectangle(0, 0, 50, 50, RED);
	testVec->x = 90.0;
	testVec->y = 200.0f;
	DrawCircle(0, 0, testVec->length(*testVec), BLUE);
	DrawLine(0, 0, testVec->x, testVec->y, GREEN);
	
	EndMode2D();
	EndDrawing();
	return 0;
}

