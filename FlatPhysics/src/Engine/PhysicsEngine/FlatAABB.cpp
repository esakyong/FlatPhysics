#include "FlatAABB.h"

namespace FlatPhysics
{
	FlatAABB::FlatAABB(const FlatVector& min, const FlatVector& max) :
		Min(min), Max(max)
	{}

	FlatAABB::FlatAABB(float minX, float minY, float maxX, float maxY) :
		Min({minX, minY}), Max({maxX, maxY})
	{}


}