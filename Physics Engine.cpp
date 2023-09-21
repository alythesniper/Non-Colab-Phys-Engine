#include <iostream>
#include <chrono>
#include "reality.h"

int main()
{
	//create main "reality"
	reality* realityOne = new reality();
	
	//game loop, exits if <esc> pressed
	while (true)
	{
		realityOne->update();
		if (IsKeyDown(KEY_ESCAPE))
		{
			delete realityOne;
			break;
		}
	}

	return 0;
}

