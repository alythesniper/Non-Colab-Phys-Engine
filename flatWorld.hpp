#pragma once

class FlatWorld
{
public:
	static const float minBodySize; //m
	static const float maxBodySize;

	static const float minDensity; // g/cm^3
	static const float maxDensity;
		
};

const float FlatWorld::minBodySize = 0.01 * 0.01;
const float FlatWorld::maxBodySize = 64 * 64;

const float minDensity = 0.2; 
const float maxDensity = 21.4;
