#pragma once
#include "Common.h"
#include "FlatConvertor.h"
#include <vector>
using FlatPhysics::FlatVector;

class GameDraw {
public:
	static void DrawPolygonLines(const std::vector<FlatVector>& points, Color color)
	{
		std::vector<Vector2> vertices;
		FlatConvertor::ToVector2Array(points, vertices);
		for (int i = 0; i < points.size(); i++)
		{
			DrawLineV(vertices[i], vertices[(i + 1) % points.size()], color);
		}
		
	}
};

