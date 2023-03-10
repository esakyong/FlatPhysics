#include "FlatWorld.h"

namespace FlatPhysics 
{

	FlatWorld::FlatWorld() : gravity(FlatVector(0.0f, 98.1f))
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

	void FlatWorld::Step(float time, int totalIterations)
	{
		totalIterations = FlatMath::Clamp(totalIterations, MinIterations(), MaxIterations());

		ContactPointVector.clear();

		for (int currentIteration = 0; currentIteration < totalIterations; currentIteration++)
		{ 
			// Movement step
			for (int i = 0; i < bodyVector.size(); i++)
			{
				bodyVector[i]->Step(time, gravity, totalIterations);
			}

			contactVector.clear();

			// collision step
			for (int i = 0; i < bodyVector.size() - 1; i++)
			{
				FlatBody* bodyA = bodyVector[i];
				FlatAABB bodyA_aabb = bodyA->GetAABB();
				
				for (int j = i + 1; j < bodyVector.size(); j++)
				{

					FlatBody* bodyB = bodyVector[j];
					FlatAABB bodyB_aabb = bodyB->GetAABB();
	
					
					if (bodyA->IsStatic && bodyB->IsStatic)
					{
						continue;
					}

					if (!Collisions::IntersectAABBs(bodyA_aabb, bodyB_aabb))
					{
						continue;
					}



					FlatVector normal;
					float depth;
					if (Collisions::Collide(bodyA, bodyB, normal, depth))
					{
						SeperateBodies(bodyA, bodyB, normal * depth);
						
						FlatVector contact1;
						FlatVector contact2;
						int contactCount;
						Collisions::FindContactPoints(bodyA, bodyB, contact1, contact2, contactCount);

						ShapeType shapeTypeA = bodyA->shapeType;
						ShapeType shapeTypeB = bodyB->shapeType;
						

						FlatManifold* contact = new FlatManifold(
							bodyA, bodyB, normal, depth,
							contact1, contact2, contactCount);

						contactVector.push_back(contact);
						
					}
					
				}
				
			}

			for (int i = 0; i < contactVector.size(); i++)
			{
				FlatManifold* contact = contactVector[i];
				ResolveCollision(*contact);

				// Add contact points for display (DEBUG).
				if (currentIteration == totalIterations - 1)
				{
					auto it = std::find(ContactPointVector.begin(), ContactPointVector.end(), &contact->Contact1);
					if (it == ContactPointVector.end())
					{
						ContactPointVector.push_back(&contact->Contact1);
					}

					if (contact->ContactCount > 1)
					{
						auto it = std::find(ContactPointVector.begin(), ContactPointVector.end(), &contact->Contact2);
						if (it == ContactPointVector.end())
						{
							ContactPointVector.push_back(&contact->Contact2);
						}
					}
				}
				
				
			}
		}

	}
	
	void FlatWorld::SeperateBodies(FlatBody* bodyA, FlatBody* bodyB, const FlatVector& mtv)
	{
		if (bodyA->IsStatic)
		{
			bodyB->Move(mtv);
		}
		else if (bodyB->IsStatic)
		{
			bodyA->Move(-mtv);
		}
		else
		{
			bodyA->Move(-mtv / 2.0f);
			bodyB->Move(mtv / 2.0f);
		}
	}

	void FlatWorld::ResolveCollision(const FlatManifold& contact)
	{
		FlatBody* bodyA = contact.BodyA;
		FlatBody* bodyB = contact.BodyB;
		FlatVector normal = contact.Normal;
		float depth = contact.Depth;

		FlatVector relativeVelocity = bodyB->LinearVelocity - bodyA->LinearVelocity;
		


		if (FlatMath::Dot(relativeVelocity, normal) > 0.0f)
		{
			return;
		}

		float e = std::min(bodyA->Restitution, bodyB->Restitution);

		float j = -(1.0f + e) * FlatMath::Dot(relativeVelocity, normal);

		j /= bodyA->InvMass + bodyB->InvMass;

		FlatVector impulse = j * normal;

		bodyA->LinearVelocity -= impulse * bodyA->InvMass;
		bodyB->LinearVelocity += impulse * bodyB->InvMass;
	}

	
	
}