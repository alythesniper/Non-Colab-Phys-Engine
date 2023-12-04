#include "reality.hpp"

int main()
{
	//create main "reality"
	reality* realityOne = new reality(true);

	//set debug output level
	SetTraceLogLevel(LOG_ERROR);
	
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

