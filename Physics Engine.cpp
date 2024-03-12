#include "reality.hpp"
#include <chrono>

auto lastTime = std::chrono::high_resolution_clock::now();

int main()
{
	//get time since last frame
	auto currentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
	
	//create main "reality"
	reality* realityOne = new reality();

	//set debug output level
	SetTraceLogLevel(LOG_ERROR);
	
	//game loop, exits if <esc> pressed
	while (true)
	{
		realityOne->update(deltaTime);
		if (IsKeyDown(KEY_ESCAPE))
		{
			delete realityOne;
			break;
		}
	}

	return 0;
}

