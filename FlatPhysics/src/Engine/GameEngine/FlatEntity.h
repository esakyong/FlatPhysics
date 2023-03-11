#pragma once
#include "Common.h"
#include "RandomHelper.h"
#include "FlatConverter.h"
using namespace FlatPhysics;

class FlatEntity
{
private:
	FlatBody* body = new FlatBody();
	Color color;

	std::vector<Vector2> vertexBuffer;
public:
	FlatBody* GetBody();
	Color GetColor();

	FlatEntity(FlatBody* body);
	FlatEntity(FlatBody* body, const Color& color);
	FlatEntity(FlatWorld& world, float radius, bool isStatic, const FlatVector& position);
	FlatEntity(FlatWorld& world, float width, float height, bool isStatic, const FlatVector& position);
	void Draw();

};
