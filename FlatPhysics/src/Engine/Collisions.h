#pragma once
#include "FlatVector.h"
#include "FlatMath.h"
#include <limits>
#include <cmath>

namespace FlatPhysics
{
	class Collisions
	{
	
	public:
		bool static IntersectCirclePolygon(const FlatVector& circleCenter, const float circleRadius, 
			const std::vector<FlatVector>& vertices,
			FlatVector& normal, float& depth)
		{
			
			depth = std::numeric_limits<float>::max();

			FlatVector axis = FlatVector::Zero();
			float axisDepth = 0.0f;
			float minA, maxA, minB, maxB;

			for (int i = 0; i < vertices.size(); i++)
			{
				FlatVector va = vertices[i];
				FlatVector vb = vertices[(i + 1) % vertices.size()];

				FlatVector edge = vb - va;
				axis = FlatVector(-edge.y, edge.x);
				axis = FlatMath::Normalize(axis);

				
				ProjectVertices(vertices, axis, minA, maxA);
				ProjectCircle(circleCenter, circleRadius, axis, minB, maxB);

				if (minA >= maxB || minB >= maxA)
				{
					return false;
				}

				axisDepth = std::min(maxB - minA, maxA - minB);

				if (axisDepth < depth)
				{
					depth = axisDepth;
					normal = axis;
				}
			}

			int cpIndex = FindClosestPointOnPolygon(circleCenter, vertices);
			FlatVector cp = vertices[cpIndex];

			axis = cp - circleCenter;
			axis = FlatMath::Normalize(axis);

			ProjectVertices(vertices, axis, minA, maxA);
			ProjectCircle(circleCenter, circleRadius, axis, minB, maxB);

			if (minA >= maxB || minB >= maxA)
			{
				return false;
			}

			axisDepth = std::min(maxB - minA, maxA - minB);

			if (axisDepth < depth)
			{
				depth = axisDepth;
				normal = axis;
			}

			
			FlatVector polygonCenter = FindArithmeticMean(vertices);

			FlatVector direction = polygonCenter - circleCenter;

			if (FlatMath::Dot(direction, normal) < 0)
			{
				normal = -normal;
			}


			return true;
		}

		bool static IntersectPolygons(const std::vector<FlatVector>& verticesA, const std::vector<FlatVector>& verticesB,
			FlatVector& normal, float& depth)
		{
			
			depth = std::numeric_limits<float>::max();
			
			for (int i = 0; i < verticesA.size(); i++)
			{
				FlatVector va = verticesA[i];
				FlatVector vb = verticesA[(i + 1) % verticesA.size()];

				FlatVector edge = vb - va;
				FlatVector axis(-edge.y, edge.x);
				axis = FlatMath::Normalize(axis);

				float minA, maxA, minB, maxB;
				ProjectVertices(verticesA, axis, minA, maxA);
				ProjectVertices(verticesB, axis, minB, maxB);

				if (minA >= maxB || minB >= maxA)
				{
					return false;
				}

				
				float axisDepth = std::min(maxB - minA, maxA - minB);

				if (axisDepth < depth)
				{
					depth = axisDepth;
					normal = axis;
				}


			}

			
			

			for (int i = 0; i < verticesB.size(); i++)
			{
				FlatVector va = verticesB[i];
				FlatVector vb = verticesB[(i + 1) % verticesB.size()];

				FlatVector edge = vb - va;
				FlatVector axis(-edge.y, edge.x);
				axis = FlatMath::Normalize(axis);

				float minA, maxA, minB, maxB;
				ProjectVertices(verticesA, axis, minA, maxA);
				ProjectVertices(verticesB, axis, minB, maxB);

				if (minA >= maxB || minB >= maxA)
				{
					return false;
				}

				float axisDepth = std::min(maxB - minA, maxA - minB);

				if (axisDepth < depth)
				{
					depth = axisDepth;
					normal = axis;
				}
			}

			

			FlatVector centerA = FindArithmeticMean(verticesA);
			FlatVector centerB = FindArithmeticMean(verticesB);

			FlatVector direction =  centerB - centerA;

			if (FlatMath::Dot(direction, normal) < 0)
			{
				normal = -normal;
			}

			return true;
		}

		bool static IntersectCircles(
			const FlatVector& centerA, const float radiusA, const FlatVector& centerB, const float radiusB,
			FlatVector& normal, float& depth)
		{
			
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

		int static FindClosestPointOnPolygon(FlatVector circleCenter, std::vector<FlatVector> vertices)
		{
			int result = -1;
			float minDistance = std::numeric_limits<float>::max();

			for (int i = 0; i < vertices.size(); i++)
			{
				FlatVector v = vertices[i];
				float distance = FlatMath::Distance(v, circleCenter);

				if (distance < minDistance)
				{
					minDistance = distance;
					result = i;
				}
			}

			return result;
		}

		void static ProjectCircle(const FlatVector& center, const float radius, const FlatVector& axis, float& min, float& max)
		{
			FlatVector direction = FlatMath::Normalize(axis);
			FlatVector directionAndRadius = direction * radius;

			FlatVector p1 = center + directionAndRadius;
			FlatVector p2 = center - directionAndRadius;

			min = FlatMath::Dot(p1, axis);
			max = FlatMath::Dot(p2, axis);

			if (min > max)
			{
				// sawp the min and max values
				float t = min;
				min = max;
				max = t;
			}
		}

		void static ProjectVertices(const std::vector<FlatVector>& vertices, const FlatVector& axis, float& min, float& max)
		{
			
			
			min = std::numeric_limits<float>::max();
			max = std::numeric_limits<float>::lowest();

			for (int i = 0; i < vertices.size(); i++)
			{
				FlatVector v = vertices[i];
				float proj = FlatMath::Dot(v, axis);

				if (proj < min) { min = proj; }
				if (proj > max) { max = proj; }
			}
		}

		static FlatVector FindArithmeticMean(std::vector<FlatVector> vertices)
		{
			float sumX = 0.0f;
			float sumY = 0.0f;

			for (int i = 0; i < vertices.size(); i++)
			{
				FlatVector v = vertices[i];
				sumX += v.x;
				sumY += v.y;
			}

			return FlatVector(sumX / (float)vertices.size(), sumY / (float)vertices.size());
		}

	};
}
