#pragma once
#include "FlatVector.h"
#include "FlatMath.h"
#include <limits>
namespace FlatPhysics
{
	class Collisions
	{
	
	public:
		bool static IntersectPolygons(const std::vector<FlatVector>& verticesA, const std::vector<FlatVector>& verticesB)
		{
			for (int i = 0; i < verticesA.size(); i++)
			{
				FlatVector va = verticesA[i];
				FlatVector vb = verticesA[(i + 1) % verticesA.size()];

				FlatVector edge = vb - va;
				FlatVector axis(-edge.y, edge.x);

				float minA, maxA, minB, maxB;
				ProjectVertices(verticesA, axis, minA, maxA);
				ProjectVertices(verticesB, axis, minB, maxB);

				if (minA >= maxB || minB >= maxA)
				{
					return false;
				}
			}

			for (int i = 0; i < verticesB.size(); i++)
			{
				FlatVector va = verticesB[i];
				FlatVector vb = verticesB[(i + 1) % verticesB.size()];

				FlatVector edge = vb - va;
				FlatVector axis(-edge.y, edge.x);

				float minA, maxA, minB, maxB;
				ProjectVertices(verticesA, axis, minA, maxA);
				ProjectVertices(verticesB, axis, minB, maxB);

				if (minA >= maxB || minB >= maxA)
				{
					return false;
				}
			}

			return true;
		}

		bool static IntersectCircles(
			const FlatVector& centerA, const float radiusA, const FlatVector& centerB, const float radiusB,
			FlatVector& normal, float& depth)
		{
			normal = FlatVector::Zero();
			depth = 0.0f;
			float distance = FlatMath::Distance(centerA, centerB);
			float radii = radiusA + radiusB;

			if (distance >= radii) {
				return false;
			}
			
			normal = FlatMath::Normalize(centerB - centerA);
			depth = radii - distance;

			return true;
		}

	private:

		void static ProjectVertices(const std::vector<FlatVector>& vertices, const FlatVector& axis, float& min, float& max)
		{
			min = FLT_MAX;
			max = FLT_MIN;

			for (int i = 0; i < vertices.size(); i++)
			{
				FlatVector v = vertices[i];
				float proj = FlatMath::Dot(v, axis);

				if (proj < min) { min = proj; }
				if (proj > max) { max = proj; }
			}
		}

	};
}
