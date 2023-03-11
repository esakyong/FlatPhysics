#pragma once
#include "FlatVector.h"

namespace FlatPhysics
{

	struct FlatAABB
	{
		FlatVector Min;
		FlatVector Max;

		FlatAABB() {};
		FlatAABB(const FlatVector& min, const FlatVector& max);
		FlatAABB(float minX, float minY, float maxX, float maxY);
	};
}