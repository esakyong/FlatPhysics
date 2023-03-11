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

		for (int currentIteration = 0; currentIteration < totalIterations; currentIteration++)
		{ 
			contactPairs.clear();
			StepBodies(time, totalIterations);
			BroadPhase();
			NarrowPhase();
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

	void FlatWorld::BroadPhase()
	{
		
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
				
				contactPairs.push_back({ i, j });

			}
		}
	}

	void FlatWorld::NarrowPhase()
	{
		for (int i = 0; i < contactPairs.size(); i++)
		{
			auto pair = contactPairs[i];
			FlatBody* bodyA = bodyVector[pair.Item1];
			FlatBody* bodyB = bodyVector[pair.Item2];

			FlatVector normal;
			float depth;
			if (Collisions::Collide(bodyA, bodyB, normal, depth))
			{
				SeperateBodies(bodyA, bodyB, normal * depth);

				FlatVector contact1;
				FlatVector contact2;
				int contactCount;
				Collisions::FindContactPoints(bodyA, bodyB, contact1, contact2, contactCount);

				FlatManifold* contact = new FlatManifold(
					bodyA, bodyB, normal, depth,
					contact1, contact2, contactCount);
				//ResolveCollisionBasic(*contact);
				ResolveCollisionWithRotation(*contact);
			}
		}
	}

	
	void FlatWorld::StepBodies(float time, int totalIterations)
	{
		for (int i = 0; i < bodyVector.size(); i++)
		{
			bodyVector[i]->Step(time, gravity, totalIterations);
		}
	}

	void FlatWorld::ResolveCollisionBasic(const FlatManifold& contact)
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

	void FlatWorld::ResolveCollisionWithRotation(const FlatManifold& contact)
	{
		FlatBody* bodyA = contact.BodyA;
		FlatBody* bodyB = contact.BodyB;
		FlatVector normal = contact.Normal;
		FlatVector contact1 = contact.Contact1;
		FlatVector contact2 = contact.Contact2;
		int contactCount = contact.ContactCount;

		float e = std::min(bodyA->Restitution, bodyB->Restitution);

		contactArray[0] = contact1;
		contactArray[1] = contact2;
		
		for (int i = 0; i < contactCount; i++)
		{
			impulseArray[i] = FlatVector::Zero();
			raArray[i] = FlatVector::Zero();
			rbArray[i] = FlatVector::Zero();
		}
		
		for (int i = 0; i < contactCount; i++)
		{
			FlatVector ra = contactArray[i] - bodyA->GetPosition();
			FlatVector rb = contactArray[i] - bodyB->GetPosition();

			raArray[i] = ra;
			rbArray[i] = rb;

			FlatVector raPerp(-ra.y, ra.x);
			FlatVector rbPerp(-rb.y, rb.x);

			FlatVector angularLinearVelocityA = raPerp * bodyA->AngularVelocity;
			FlatVector angularLinearVelocityB = raPerp * bodyB->AngularVelocity;


			FlatVector relativeVelocity = 
				(bodyB->LinearVelocity + angularLinearVelocityB) - 
				(bodyA->LinearVelocity + angularLinearVelocityA);

			float contactVelocityMag = FlatMath::Dot(relativeVelocity, normal);

			if (contactVelocityMag > 0.0f)
			{
				continue;
			}

			float raPerpDotN = FlatMath::Dot(raPerp, normal);
			float rbPerpDotN = FlatMath::Dot(rbPerp, normal);

			float denom = bodyA->InvMass + bodyB->InvMass +
				(raPerpDotN * raPerpDotN) * bodyA->InvInertia +
				(rbPerpDotN * rbPerpDotN) * bodyB->InvInertia;

			float j = -(1.0f + e) * contactVelocityMag;
			j /= denom;
			j /= (float)contactCount;

			FlatVector impulse = j * normal;
			impulseArray[i] = impulse;
		}

		for (int i = 0; i < contactCount; i++)
		{
			
			FlatVector impulse = impulseArray[i];
			FlatVector ra = raArray[i];
			FlatVector rb = rbArray[i];

			bodyA->LinearVelocity += -impulse * bodyA->InvMass;
			bodyA->AngularVelocity += -FlatMath::Cross(ra, impulse) * bodyA->InvInertia;
			bodyB->LinearVelocity += impulse * bodyB->InvMass;
			bodyB->AngularVelocity += FlatMath::Cross(rb, impulse) * bodyB->InvInertia;
		}

	}

	
}