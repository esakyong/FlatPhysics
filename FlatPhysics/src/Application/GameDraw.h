#pragma once
#include "Common.h"
#include "FlatConvertor.h"
#include <vector>
using FlatPhysics::FlatVector;

class GameDraw {
public:
	static void DrawPolygonLines(const std::vector<Vector2> points, Color color)
	{
		
		for (int i = 0; i < points.size(); i++)
		{
			DrawLineV(points[i], points[(i + 1) % points.size()], color);
		}
		
	}
};

