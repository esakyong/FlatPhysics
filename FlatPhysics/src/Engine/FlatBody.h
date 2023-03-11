#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "FlatTransform.h"
#include "FlatVector.h"
#include "FlatMath.h"
#include "FlatWorld.h"
#include "FlatAABB.h"

#define PI 3.141592f

namespace FlatPhysics {
	
	class FlatWorld;

	enum class ShapeType {
		Circle = 0,
		Box = 1
	};


	class FlatBody {
	
	private:
		
		FlatVector position;
		
		float angle;
		

		FlatVector force;
		
		std::vector<FlatVector> vertices;
		
		std::vector<FlatVector> transformedVertices;
		FlatAABB aabb;

		bool transformUpdateRequired;
		bool aabbUpdateRequired;

	public:
		ShapeType shapeType;
		float Density;
		float Mass;
		float InvMass;
		float Restitution;
		float Area;
		float Inertia;
		float InvInertia;
		bool IsStatic;
		float Radius;
		float Width;
		float Height;
		float StaticFriction;
		float DynamicFriction;

		FlatVector LinearVelocity;
		float AngularVelocity;

		FlatVector GetPosition() const;
		float GetAngle() const;
		
		FlatBody();

		FlatBody(float density, float mass, float inertia, float restitution, float area,
			bool isStatic, float radius, float width, float height,
			const std::vector<FlatVector>& vertices, ShapeType shapeType);
	private:

		//std::vector<int> CreateBoxTriangles();
		
		static std::vector<FlatVector> CreateBoxVertices(float width, float height);

	public:
		std::vector<FlatVector> GetTransformedVertices();

		FlatAABB GetAABB();

		void Step(float time, const FlatVector& gravity, int iterations);

		void Move(const FlatVector& amount);

		void MoveTo(const FlatVector& position);
		
		void Rotate(float amount);

		void RotateTo(float amount);

		void AddForce(const FlatVector& amount);

		static bool CreateCircleBody(float radius, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage);

		static bool CreateBoxBody(float width, float height, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage);
	};

}