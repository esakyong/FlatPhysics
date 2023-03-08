#include "FlatWorld.h"

namespace FlatPhysics {

	FlatWorld::FlatWorld() : gravity(FlatVector(0.0f, 9.81f))
	{

	}

	FlatWorld::~FlatWorld()
	{
		for (auto& body : bodyVector)
		{
			delete body;
		}
	}

	int FlatWorld::BodyCount()
	{
		return bodyVector.size();
	}

	void FlatWorld::AddBody(FlatBody* body)
	{
		bodyVector.push_back(body);
	}

	void FlatWorld::RemoveBody(FlatBody* body)
	{
		bodyVector.erase(remove(bodyVector.begin(), bodyVector.end(), body), bodyVector.end());
	}

	bool FlatWorld::GetBody(int index, FlatBody*& body)
	{
		

		if (index < 0 || index >= bodyVector.size())
		{
			return false;
		}
		body = bodyVector[index];
		return true;
	}

	void FlatWorld::Step(float time)
	{
		// Movement step
		for (int i = 0; i < bodyVector.size(); i++)
		{
			bodyVector[i]->Step(time);
		}

		// collision step
		for (int i = 0; i < bodyVector.size() - 1; i++)
		{
			FlatBody* bodyA = bodyVector[i];

			for (int j = i + 1; j < bodyVector.size(); j++)
			{

				FlatBody* bodyB = bodyVector[j];
				FlatVector normal;
				float depth;
				if (Collide(bodyA, bodyB, normal, depth))
				{
					
					bodyA->Move(-normal * depth / 2.0f);
					bodyB->Move(normal * depth / 2.0f);

					ResolveCollision(bodyA, bodyB, normal, depth);
				}
			}
		}
	}

	void FlatWorld::ResolveCollision(FlatBody* bodyA, FlatBody* bodyB, FlatVector normal, float depth)
	{
		FlatVector relativeVelocity = bodyB->LinearVelocity - bodyA->LinearVelocity;
		
		float e = std::min(bodyA->Restitution, bodyB->Restitution);

		float j = -(1.0f + e) * FlatMath::Dot(relativeVelocity, normal);
		j /= (1.0f / bodyA->Mass + 1.0f / bodyB->Mass);

		bodyA->LinearVelocity -= j / bodyA->Mass * normal;
		bodyB->LinearVelocity += j / bodyB->Mass * normal;
	}

	bool FlatWorld::Collide(FlatBody* bodyA, FlatBody* bodyB, FlatVector& normal, float& depth)
	{
		normal = FlatVector::Zero();
		depth = 0.0f;

		ShapeType shapeTypeA = bodyA->shapeType;
		ShapeType shapeTypeB = bodyB->shapeType;

		if (shapeTypeA == ShapeType::Box)
		{
			if (shapeTypeB == ShapeType::Box)
			{
				return Collisions::IntersectPolygons(bodyA->GetTransformedVertices(), bodyB->GetTransformedVertices(),
					normal, depth);
			}

			else if (shapeTypeB == ShapeType::Circle)
			{
				bool result = Collisions::IntersectCirclePolygon(bodyB->GetPosition(), bodyB->Radius, bodyA->GetTransformedVertices(),
					normal, depth);
				normal = -normal;
				return result;
			}
		}

		else if (shapeTypeA == ShapeType::Circle)
		{
			if (shapeTypeB == ShapeType::Box)
			{
				return Collisions::IntersectCirclePolygon(bodyA->GetPosition(), bodyA->Radius, bodyB->GetTransformedVertices(),
					normal, depth);
			}

			else if (shapeTypeB == ShapeType::Circle)
			{
				return Collisions::IntersectCircles(bodyA->GetPosition(), bodyA->Radius, bodyB->GetPosition(), bodyB->Radius,
					normal, depth);
			}
		}

		return false;
	}
}