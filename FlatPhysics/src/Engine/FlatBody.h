#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "FlatTransform.h"
#include "FlatVector.h"
#include "FlatMath.h"
#include "FlatWorld.h"

#define PI 3.141592

namespace FlatPhysics {
	
	class FlatWorld;

	enum class ShapeType {
		Circle = 0,
		Box = 1
	};


	class FlatBody {
	private:
		FlatVector position;
		
		float rotation;
		float rotationalVelocity;

		FlatVector force;

		std::vector<FlatVector> vertices;
		
		std::vector<FlatVector> transformedVertices;

		bool transformUpdateRequired;

	public:
		float Density;
		float Mass;
		float InvMass;
		float Restitution;
		float Area;
		
		bool IsStatic;
		
		float Radius;
		float Width;
		float Height;
		
		FlatVector LinearVelocity;

		std::vector<int> Triangles;

		ShapeType shapeType;

		FlatVector GetPosition() const;
		
		FlatBody() {}

		FlatBody(FlatVector position, float density, float mass, float restitution, float area,
			bool isStatic, float radius, float width, float height, ShapeType shapeType);
	private:

		std::vector<int> CreateBoxTriangles();
		std::vector<FlatVector> CreateBoxVertices(float width, float height);

	public:
		std::vector<FlatVector> GetTransformedVertices();

		void Step(float time, const FlatVector& gravity);

		void Move(const FlatVector& amount);

		void MoveTo(const FlatVector& position);
		
		void Rotate(float amount);

		void AddForce(const FlatVector& amount);

		static bool CreatCircleBody(float radius, const FlatVector& position, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage);

		static bool CreatBoxBody(float width, float height, const FlatVector& position, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage);
	};

}