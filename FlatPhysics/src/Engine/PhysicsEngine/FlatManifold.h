#pragma once
#include "FlatBody.h"

namespace FlatPhysics
{
	class FlatBody;

	struct FlatManifold
	{
		FlatBody* BodyA;
		FlatBody* BodyB;
		FlatVector Normal;
		float Depth;
		FlatVector Contact1;
		FlatVector Contact2;
		int ContactCount;

		FlatManifold() {}

		FlatManifold(
			FlatBody* bodyA, FlatBody* bodyB,
			const FlatVector& normal, float depth,
			const FlatVector& contact1, const FlatVector& contact2, int contactCount);
		
	};
}
