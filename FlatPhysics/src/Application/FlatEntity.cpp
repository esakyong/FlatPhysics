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
		Vector2 va = { 0.0f, 0.0f };
		Vector2 vb = { body->Radius, 0.0f };
		Matrix matrix1 = MatrixTranslate(body->GetPosition().x, body->GetPosition().y, 0.0f);
		Matrix matrix2 = MatrixRotateZ(body->GetAngle());
		Matrix matrix = MatrixMultiply(matrix2, matrix1);
		va = Vector2Transform(va, matrix);
		vb = Vector2Transform(vb, matrix);

		DrawCircleV(position, body->Radius, color);
		DrawRing(position, body->Radius - 0.7f, body->Radius, 0, 360, 100, WHITE);
		DrawLineV(va, vb, WHITE);
	}
	else if (body->shapeType == ShapeType::Box)
	{
		std::vector<FlatVector> vertices = body->GetTransformedVertices();
		FlatConverter::ToVector2Array(vertices, vertexBuffer);

		DrawTriangleFan(vertexBuffer.data(), vertexBuffer.size(), color);
		GameDraw::DrawPolygonLines(vertexBuffer, 0.7f, WHITE);
	}
}
