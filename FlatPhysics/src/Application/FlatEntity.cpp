#include "FlatEntity.h"

FlatBody* FlatEntity::GetBody() { return body; }
Color FlatEntity::GetColor() { return color; }

FlatEntity::FlatEntity(FlatBody* body) :
	body(body), color(RandomHelper::RandomColor()) {}

FlatEntity::FlatEntity(FlatBody* body, const Color& color) :
	body(body), color(color) {}

FlatEntity::FlatEntity(FlatWorld& world, float radius, bool isStatic, const FlatVector& position) :
	color(RandomHelper::RandomColor())
{
	
	std::string errorMessage;
	if (!FlatBody::CreateCircleBody(radius, 1.0f, isStatic, 0.5f,
		*body, errorMessage))
	{
		throw std::invalid_argument(errorMessage);
	}

	body->MoveTo(position);
	this->body = body;
	world.AddBody(this->body);
}

FlatEntity::FlatEntity(FlatWorld& world, float width, float height, bool isStatic, const FlatVector& position) :
	color(RandomHelper::RandomColor())
{
	
	std::string errorMessage;
	if (!FlatBody::CreateBoxBody(width, height, 1.0f, isStatic, 0.5f,
		*body, errorMessage))
	{
		throw std::invalid_argument(errorMessage);
	}

	body->MoveTo(position);
	this->body = body;
	world.AddBody(this->body);
}

void FlatEntity::Draw() 
{
	Vector2 position = FlatConverter::ToVector2(body->GetPosition());
	if (body->shapeType == ShapeType::Circle)
	{
		DrawCircleV(position, body->Radius, color);
		DrawRing(position, body->Radius - 0.7f, body->Radius, 0, 360, 100, WHITE);
	}
	else if (body->shapeType == ShapeType::Box)
	{
		std::vector<FlatVector> vertices = body->GetTransformedVertices();
		FlatConverter::ToVector2Array(vertices, vertexBuffer);

		DrawTriangleFan(vertexBuffer.data(), vertexBuffer.size(), color);
		GameDraw::DrawPolygonLines(vertexBuffer, 0.7f, WHITE);
	}
}
