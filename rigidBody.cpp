#include "rigidBody.h"

rigidBody::rigidBody(FlatVector positioni, float densityi, float massi, float restitutioni,
	float areai, bool isStatici, float radiusi, float widthi, float heighti,
	shapeType ShapeTypei):
	position(positioni), density(densityi),mass(massi),
	restitution(restitutioni),area(areai),isStatic(isStatici),radius(radiusi),
	width(widthi),height(heighti),bodyShapeType(ShapeTypei)
{
	
	
}
