#pragma once
#include <vector>

#include "FlatVector.h"
#include "Collisions.h"
#include "FlatBody.h"

namespace FlatPhysics {
	
	class FlatBody;

	class FlatWorld {
	public:
		static float MinBodySize() { return 0.01f * 0.01f; }
		static float MaxBodySize() { return 64.0f * 64.0f; }

		static float MinDensity() { return 0.5f; }	// g/cm^3
		static float MaxDensity() { return 21.4f; }
	private:
		std::vector<FlatBody*> bodyVector;
		FlatVector gravity;

	public:
		FlatWorld();
		~FlatWorld();

		int BodyCount();

		void AddBody(FlatBody* body);

		void RemoveBody(FlatBody* body);

		bool GetBody(int index, FlatBody* &body);

		void Step(float time);
		
		void ResolveCollision(FlatBody* bodyA, FlatBody* bodyB, FlatVector normal, float depth);

		bool Collide(FlatBody* bodyA, FlatBody* bodyB, FlatVector& normal, float& depth);
	};
}
