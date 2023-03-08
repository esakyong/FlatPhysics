#pragma once

#include <cmath>
#include "FlatVector.h"

namespace FlatPhysics {
	class FlatMath
	{
	public:
		static float Clamp(float value, float min, float max)
		{
			if (min == max)
			{
				return min;
			}

			if (min > max)
			{
				// TODO:throw error
			}
			if (value < min)
			{
				return min;
			}
			if (value > max)
			{
				return max;
			}

			return value;
		}
		static float Length(const FlatVector& v) {
			return sqrt(v.x * v.x + v.y * v.y);
		}
		static float Distance(const FlatVector& a, const FlatVector& b) {
			float dx = a.x - b.x;
			float dy = a.y - b.y;
			return sqrt(dx * dx + dy * dy);
		}

		static FlatVector Normalize(const FlatVector& v) {
			float len = Length(v);
			return FlatVector(v.x / len, v.y / len);
		}

		static float Dot(const FlatVector& a, const FlatVector& b) {
			return a.x * b.x + a.y * b.y;
		}

		static float Cross(const FlatVector& a, const FlatVector& b) {
			return a.x * b.y - a.y * b.x;
		}
	};
	
}