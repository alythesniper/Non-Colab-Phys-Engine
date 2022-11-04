#include "reality.h"

reality::reality() 
{
	//create window and enable VSYNC
	InitWindow(800, 600, "Physics Engine");
    SetWindowState(FLAG_VSYNC_HINT);
	camera.zoom = 1.0f;
	camera.target = { 0.0f, 0.0f};
	
	//make screen origin center of the window
	camera.offset = { 400, 300 };
}


int reality::update()
{
	//exit engine if <esc>
	if (IsKeyDown(KEY_ESCAPE))
	{
		CloseWindow();
		return 1;
	}
	
	//camera rotation controlls
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
		DrawRectangle(0, 0, 50, 50, RED);
	EndMode2D();
	EndDrawing();
	
	
	
	return 0;
}

