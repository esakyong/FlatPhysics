#include "FlatBody.h"

namespace FlatPhysics
{
	FlatVector FlatBody::GetPosition() const 
	{
		return position;
	}

	float FlatBody::GetAngle() const
	{
		return angle;
	}

	
	FlatBody::FlatBody() {}

	FlatBody::FlatBody(float density, float mass, float inertia, float restitution, float area,
		bool isStatic, float radius, float width, float height,
		const std::vector<FlatVector>& vertices, ShapeType shapeType) :
		position(FlatVector::Zero()), LinearVelocity(FlatVector::Zero()), angle(0.0f), AngularVelocity(0.0f),
		Density(density), Mass(mass), InvMass(mass > 0.0f ? 1.0f / mass : 0.0f),
		Inertia(inertia), InvInertia(inertia > 0.0f ? 1.0f / inertia : 0.0f), Restitution(restitution), Area(area),
		IsStatic(isStatic), Radius(radius), Width(width), Height(height), shapeType(shapeType),
		transformUpdateRequired(true), aabbUpdateRequired(true), StaticFriction(0.6f), DynamicFriction(0.4f)
	{
		

		if (shapeType == ShapeType::Box)
		{
			this->vertices = vertices;
			transformedVertices.resize(vertices.size());
		}
		else
		{
			this->vertices.clear();
			transformedVertices.clear();
		}

		
	}


	//std::vector<int> FlatBody::CreateBoxTriangles()
	//{
	//	std::vector<int> triangles = { 0, 1, 2, 0, 2, 3 };
	//	return triangles;
	//
	//}


	std::vector<FlatVector> FlatBody::CreateBoxVertices(float width, float height)
	{
		
		
		float left = -width / 2.0f;
		float right = left + width;
		float bottom = -height / 2.0f;
		float top = bottom + height;

		std::vector<FlatVector> vertices = {
			{ left, top },
			{ right, top },
			{ right, bottom },
			{ left, bottom }
		};

		return vertices;
	}


	std::vector<FlatVector> FlatBody::GetTransformedVertices()
	{

		if (transformUpdateRequired)
		{
			FlatTransform transform(position, angle);
			for (int i = 0; i < vertices.size(); i++)
			{
				FlatVector v = vertices[i];
				transformedVertices[i] = FlatVector::Transform(v, transform);
			}
		}

		transformUpdateRequired = false;
		return transformedVertices;
	}

	FlatAABB FlatBody::GetAABB()
	{
		if(aabbUpdateRequired)
		{
			float minX = std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::lowest();
			float maxY = std::numeric_limits<float>::lowest();
			if (shapeType == ShapeType::Box)
			{
				std::vector<FlatVector> vertices = GetTransformedVertices();

				for (int i = 0; i < vertices.size(); i++)
				{
					FlatVector v = vertices[i];

					if (v.x < minX) { minX = v.x; }
					if (v.x > maxX) { maxX = v.x; }
					if (v.y < minY) { minY = v.y; }
					if (v.y > maxY) { maxY = v.y; }
				}
			}
			else if (shapeType == ShapeType::Circle)
			{
				minX = position.x - Radius;
				minY = position.y - Radius;
				maxX = position.x + Radius;
				maxY = position.y + Radius;
			}
			else
			{
				throw std::invalid_argument("Unknown ShapeType.");
			}

			aabb = FlatAABB( minX, minY, maxX, maxY );
		}
		aabbUpdateRequired = false;
		return aabb;
	}

	void FlatBody::Step(float time, const FlatVector& gravity, int iterations)
	{
		
		if (IsStatic)
		{
			return;
		}

		time /= (float)iterations;

		LinearVelocity += gravity * time;
		
		position += LinearVelocity * time;
		

		angle += AngularVelocity * time;
		
		force = FlatVector::Zero();

		transformUpdateRequired = true;
		aabbUpdateRequired = true;
		
	}

	void FlatBody::Move(const FlatVector& amount)
	{
		
		position += amount;
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void FlatBody::MoveTo(const FlatVector& position)
	{
		this->position = position;
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void FlatBody::Rotate(float amount)
	{
		angle += amount;
		if (angle > 2 * PI)
		{
			angle -= 2 * PI;
		}
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void FlatBody::RotateTo(float amount)
	{
		angle = amount;
		
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void FlatBody::AddForce(const FlatVector& amount)
	{
		force = amount;
	}

	bool FlatBody::CreateCircleBody(float radius, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage) {

		errorMessage = { 0 };
		
		float area = radius * radius * PI;

		if (area < FlatWorld::MinBodySize()) {
			errorMessage = "Circle radius is too small. Min circle area is " + std::to_string(FlatWorld::MinBodySize()) + ".";
			return false;
		}

		if (area > FlatWorld::MaxBodySize()) {
			errorMessage = "Circle radius is too large. Max circle area is " + std::to_string(FlatWorld::MaxBodySize()) + ".";
			return false;
		}

		if (density < FlatWorld::MinDensity()) {
			errorMessage = "Density radius is too small. Min density area is " + std::to_string(FlatWorld::MinDensity()) + ".";
			return false;
		}

		if (density > FlatWorld::MaxDensity()) {
			errorMessage = "Density radius is too large. Min density area is " + std::to_string(FlatWorld::MaxDensity()) + ".";
			return false;
		}

		restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);

		float mass = 0.0f;
		float inertia = 0.0f;

		if (!isStatic)
		{
			mass = area * density;
			inertia = (1.0f / 2.0f) * mass * (radius * radius);
		}
		
		body = FlatBody(density, mass, inertia, restitution, area, isStatic, radius, 0.0f, 0.0f, std::vector<FlatVector>(), ShapeType::Circle);
		return true;
	}

	bool FlatBody::CreateBoxBody(float width, float height, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage) {

		errorMessage = { 0 };

		float area = width * height;

		if (area < FlatWorld::MinBodySize()) {
			errorMessage = "Area is too small. Min area is " + std::to_string(FlatWorld::MinBodySize()) + ".";
			return false;
		}

		if (area > FlatWorld::MaxBodySize()) {
			errorMessage = "Area is too large. Max area area is " + std::to_string(FlatWorld::MaxBodySize()) + ".";
			return false;
		}

		if (density < FlatWorld::MinDensity()) {
			errorMessage = "Density radius is too small. Min density area is " + std::to_string(FlatWorld::MinDensity()) + ".";
			return false;
		}

		if (density > FlatWorld::MaxDensity()) {
			errorMessage = "Density radius is too large. Min density area is " + std::to_string(FlatWorld::MaxDensity()) + ".";
			return false;
		}

		restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);

		float mass = 0.0f;
		float inertia = 0.0f;

		if (!isStatic)
		{
			mass = area * density;
			inertia = (1.0f / 12.0f) * mass * (width * width + height * height);
		}
		
		std::vector<FlatVector> vertices = CreateBoxVertices(width, height);
		body = FlatBody(density, mass, inertia, restitution, area, isStatic, 0.0f, width, height, vertices, ShapeType::Box);
		return true;
	}
}