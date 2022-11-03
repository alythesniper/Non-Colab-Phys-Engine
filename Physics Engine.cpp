#include <iostream>
#include "reality.h"

int main()
{
	//create main "reality"
	reality realityOne = reality();
	
	//game loop, returns one if <esc>
	while (true)
	{
		if (realityOne.update() == 1)
		{
			break;
		}
	}

	return 0;
}

