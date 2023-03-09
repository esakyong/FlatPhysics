#pragma once

#include <string>
#include <vector>
#include "Common.h"
#include "FlatConverter.h"


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
	void Update(float time);
	void Draw(float time);
	void End();

private:
	std::string errorMessage;

	const float defaultZoom = 2.4f;
	CameraManager camera
	{ 
		{ 
			{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 },
			{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 },
			0.0f, defaultZoom
		}, 
		70.0f, 0.5f
	};

	FlatBody* goundBody = new FlatBody();
	
	std::vector<Color> colors;
	std::vector<Color> outlineColors;
	
	std::vector<Vector2> vertexBuffer;


	FlatWorld world;


	void WrapScreen();
};



