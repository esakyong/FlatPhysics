#pragma once
#include "../Engine.h"
#include "raylib.h"
using namespace FlatPhysics;

class FlatConvertor {
public:
	static Vector2 ToVector2(FlatVector v) {
		return Vector2{ v.x, v.y };
	}

	static void ToVector2Array(std::vector<FlatVector> src, std::vector<Vector2>& dst)
	{
		if (dst.empty() || src.size() != dst.size())
		{
			dst.resize(src.size());
		}

		for (int i = 0; i < src.size(); i++)
		{
			FlatVector v = src[i];
			dst[i] = Vector2{ v.x, v.y };
		}
	}
};