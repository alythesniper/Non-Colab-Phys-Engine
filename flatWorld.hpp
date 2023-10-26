#pragma once

class FlatWorld
{
public:
	static const float minBodySize; //m
	static const float maxBodySize;

	static const float minDensity; // g/cm^3
	static const float maxDensity;
		
};


//initialising static variables
const float FlatWorld::minBodySize = 0.01 * 0.01;
const float FlatWorld::maxBodySize = 10000 * 10000;
const float FlatWorld::minDensity = 0.2; 
const float FlatWorld::maxDensity = 21.4;
