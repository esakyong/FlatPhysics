#pragma once

#include "Common.h"
#include <string>
#include <vector>

using namespace FlatPhysics;

struct CameraExtents {
	float left;
	float right;
	float bottom;
	float top;
};

struct CameraManager {
	Camera2D camera;
	float linearSpeed;
	float zoomSpeed;
	CameraExtents GetExtents();
};

class Application
{
public:
	void Setting();
	void Update();
	void Draw();
	void End();
	std::string errorMessage;

	const float defaultZoom = 24.0f;
	CameraManager camera
	{ 
		{ 
			{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 }, 
			{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 }, 
			0.0f, defaultZoom
		}, 
		70.0f, 0.5f
	};

	int bodyCount = 10;
	std::vector<FlatBody*> bodyVector;
	Color* colors = new Color[bodyCount];
	Color* outlineColors = new Color[bodyCount];
	//Color outlineColors[10];
	//Color colors[10];
	std::vector<Vector2> vertexBuffer;
};

inline Application app;

