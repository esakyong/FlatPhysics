#pragma once
#include "Common.h"
#include <string>



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

class Game
{
public:
	void Setting();
	void Update(float time);
	void Draw(float time);
	void End();

private:

	std::string errorMessage;

	const float defaultZoom = 2.0f;
	CameraManager camera
	{ 
		{ 
			{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 },
			{ 0, 0 },
			0.0f, defaultZoom
		}, 
		70.0f, 0.5f
	};

	
	
	std::vector<FlatEntity*> entityVector;
	std::vector<FlatEntity*> entityRemovalVector;

	FlatWorld world;

};



