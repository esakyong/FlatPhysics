#pragma once
#include <vector>
#include <algorithm>
#include "FlatVector.h"
#include "Collisions.h"
#include "FlatBody.h"
#include "FlatManifold.h"

namespace FlatPhysics {
	
	class FlatBody;
	struct FlatManifold;
	class FlatWorld {
	public:
		static float MinBodySize() { return 0.1f * 0.1f; }
		static float MaxBodySize() { return 640.0f * 640.0f; }

		static float MinDensity() { return 0.5f; }	// g/cm^3
		static float MaxDensity() { return 21.4f; }

		static int MinIterations() { return 1; }
		static int MaxIterations() { return 128; }

		std::vector<FlatVector*> ContactPointVector;

	private:

		std::vector<FlatBody*> bodyVector;
		FlatVector gravity;
		std::vector<FlatManifold*> contactVector;

		

	public:
		FlatWorld();
		~FlatWorld();

		int BodyCount();

		void AddBody(FlatBody* body);

		void RemoveBody(FlatBody* body);

		bool GetBody(int index, FlatBody* &body);

		void Step(float time, int iterations);
	private:
		void SeperateBodies(FlatBody* bodyA, FlatBody* bodyB, const FlatVector& mtv);
	public:
		void ResolveCollision(const FlatManifold& contact);
	};
}
