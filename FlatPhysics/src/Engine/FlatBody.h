#pragma once
#include "FlatTransform.h"
#include "FlatVector.h"
#include "FlatWorld.h"
#include "FlatMath.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#define PI 3.141592

namespace FlatPhysics {

	enum class ShapeType {
		Circle = 0,
		Box = 1
	};

	class FlatBody {
	private:
		FlatVector position;
		FlatVector linearVelocity;
		float rotation;
		float rotationalVelocity;

		std::vector<FlatVector> vertices;
		
		std::vector<FlatVector> transformedVertices;

		bool transformUpdateRequired;

	public:
		float Density;
		float Mass;
		float Restitution;
		float Area;
		
		bool IsStatic;
		
		float Radius;
		float Width;
		float Height;
		
		std::vector<int> Triangles;

		ShapeType shapeType;

		FlatVector GetPosition() const {
			return position;
		}

		FlatBody() {}

		FlatBody(FlatVector position, float density, float mass, float restitution, float area,
			bool isStatic, float radius, float width, float height, ShapeType shapeType) :
			position(position), linearVelocity(FlatVector::Zero()), rotation(0.0f), rotationalVelocity(0.0f),
			Density(density), Mass(mass), Restitution(restitution), Area(area),
			IsStatic(isStatic), Radius(radius), Width(width), Height(height), shapeType(shapeType)
		{
			if (shapeType == ShapeType::Box)
			{
				vertices = CreateBoxVertices(Width, Height);
				Triangles = CreateBoxTriangles();
				transformedVertices.resize(vertices.size());
			}
			else
			{
				vertices.clear();
				Triangles.clear();
				transformedVertices.clear();
			}

			transformUpdateRequired = true;
		}
	private:

		std::vector<int> CreateBoxTriangles()
		{
			std::vector<int> triangles = { 0, 1, 2, 0, 2, 3 };
			return triangles;
			
		}
		std::vector<FlatVector> CreateBoxVertices(float width, float height)
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

	public:
		std::vector<FlatVector> GetTransformedVertices()
		{
			if (transformUpdateRequired)
			{
				FlatTransform transform(position, rotation);
				for (int i = 0; i < vertices.size(); i++)
				{
					FlatVector v = vertices[i];
					transformedVertices[i] = FlatVector::Transform(v, transform);
				}
			}

			transformUpdateRequired = false;
			return transformedVertices;
		}

		void Move(FlatVector amount)
		{
			position += amount;
			transformUpdateRequired = true;
		}

		void MoveTo(FlatVector position)
		{
			this->position = position;
			transformUpdateRequired = true;
		}
		
		void Rotate(float amount)
		{
			rotation += amount;
			transformUpdateRequired = true;
		}

		static bool CreatCircleBody(float radius, FlatVector position, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage) {
			
			errorMessage = { 0 };

			float area = radius * radius * PI;

			if (area < FlatWorld::MinBodySize) {
				errorMessage = "Circle radius is too small. Min circle area is " + std::to_string(FlatWorld::MinBodySize) + ".";
				return false;
			}

			if (area > FlatWorld::MaxBodySize) {
				errorMessage = "Circle radius is too large. Max circle area is " + std::to_string(FlatWorld::MaxBodySize) + ".";
				return false;
			}

			if (density < FlatWorld::MinDensity) {
				errorMessage = "Density radius is too small. Min density area is " + std::to_string(FlatWorld::MinDensity) + ".";
				return false;
			}

			if (density > FlatWorld::MaxDensity) {
				errorMessage = "Density radius is too large. Min density area is " + std::to_string(FlatWorld::MaxDensity) + ".";
				return false;
			}

			restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);
			float mass = area * density;

			body = FlatBody(position, density, mass, restitution, area, isStatic, radius, 0.0f, 0.0f, ShapeType::Circle);
		}

		static bool CreatBoxBody(float width, float height, FlatVector position, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage) {

			errorMessage = { 0 };

			float area = width * height;

			if (area < FlatWorld::MinBodySize) {
				errorMessage = "Area is too small. Min area is " + std::to_string(FlatWorld::MinBodySize) + ".";
				return false;
			}

			if (area > FlatWorld::MaxBodySize) {
				errorMessage = "Area is too large. Max area area is " + std::to_string(FlatWorld::MaxBodySize) + ".";
				return false;
			}

			if (density < FlatWorld::MinDensity) {
				errorMessage = "Density radius is too small. Min density area is " + std::to_string(FlatWorld::MinDensity) + ".";
				return false;
			}

			if (density > FlatWorld::MaxDensity) {
				errorMessage = "Density radius is too large. Min density area is " + std::to_string(FlatWorld::MaxDensity) + ".";
				return false;
			}

			restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);
			float mass = area * density;

			body = FlatBody(position, density, mass, restitution, area, isStatic, 0.0f, width, height, ShapeType::Box);
			return true;
		}
	};

}