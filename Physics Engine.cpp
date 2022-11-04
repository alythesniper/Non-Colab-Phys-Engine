#include <iostream>
#include <chrono>
#include "reality.h"

int main()
{
	FlatVector vec1 = { 5, 8 };
	FlatVector vec2 = { 3, 1 };

	float f1 = 3;
	float f2 = 9;
	
	auto start = std::chrono::high_resolution_clock::now();

	for (uint64_t i = 0; i < 17'844'674'407'709'551'716; i++)
	{
		vec2 = (vec2 + vec1);
	}

//	for (int i = 0; i < 1'000'000; i++)
//	{
//		f2 += f1;
//		f1 += f2;
//	}
	
		
	auto end = std::chrono::high_resolution_clock::now();
	auto timeTaken_ms = double(std::chrono::duration_cast <std::chrono::nanoseconds> (end - start).count());
	std::cout << "Time taken: " << timeTaken_ms/1'000'000;
	//takes about 0.0001 m/s to run 17844674407709551716 float addition and assignments... c++ is better than python 
	
	return 0;
	
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

