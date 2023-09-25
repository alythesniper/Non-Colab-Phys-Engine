#include "flatVector.h"

using std::string;

//constructor
FlatVector::FlatVector(float X, float Y)
{
	x = X;
	y = Y;
}

//overloading basic vector sum & difference operations
FlatVector FlatVector::operator + (const FlatVector& b)
{
	return FlatVector(x + b.x, y + b.y);
}
FlatVector FlatVector::operator - (const FlatVector& b)
{
	return FlatVector(x - b.x, y - b.y);
}

//overloading basic vector product & quotient operations
FlatVector FlatVector::operator * (const float& b)
{
	return FlatVector(x*b, y*b);
}
FlatVector FlatVector::operator / (const float& b)
{
	return FlatVector(x/b, y/b);
}

//invert/flip the direction of a vector
FlatVector FlatVector::operator - ()
{
	return FlatVector(-x, -y);
}

//overloading equality operator
bool FlatVector::operator == (FlatVector& b)
{
	//checks if operand is FlatVector Type
	if (typeid(b) == typeid(FlatVector))
	{
		if (x == b.x && y == b.y) { return true; }
		else { return false; }
	}
	//otherwise return 0
	else
	{
		return 0;
		//add error to future logging system
	}
}

