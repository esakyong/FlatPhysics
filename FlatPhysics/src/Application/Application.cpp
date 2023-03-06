#include "Application.h"

#include "FlatConvertor.h"
#include "RandomHelper.h"
#include "GameDraw.h"


CameraExtents CameraManager::GetExtents()
{
    float left = camera.target.x - GetScreenWidth() / camera.zoom / 2;
    float right = camera.target.x + GetScreenWidth() / camera.zoom / 2;
    float bottom = camera.target.y - GetScreenHeight() / camera.zoom / 2;
    float top = camera.target.y + GetScreenHeight() / camera.zoom / 2;

    return { left, right, bottom, top };
}

void Application::Setting() {

    CameraExtents CameraExtent = camera.GetExtents();
    
    float padding = (CameraExtent.right - CameraExtent.left) * 0.05f;
    
    for (int i = 0; i < bodyCount; i++)
    {
        
        int type = RandomHelper::RandomInteger(0, 1);
        //type = (int)ShapeType::Circle;
        type = (int)ShapeType::Box;
        FlatBody* body = new FlatBody();
        
        float x = RandomHelper::RandomFloat(CameraExtent.left + padding, CameraExtent.right - padding);
        float y = RandomHelper::RandomFloat(CameraExtent.bottom + padding, CameraExtent.top - padding);

        if (type == (int)ShapeType::Circle)
        {
            if (!FlatBody::CreatCircleBody(10.0f, FlatVector(x, y), 2.0f, false, 0.5f, *body, errorMessage))
            {
                // throw new Exceptions
            }
        }
        else if (type == (int)ShapeType::Box)
        {
            if (!FlatBody::CreatBoxBody(2.0f, 2.0f, FlatVector(x, y), 2.0f, false, 0.5f, *body, errorMessage))
            {
                // throw new Exceptions
            }
        }
        else
        {
            // throw error unknown type
        }

        bodyVector.push_back(body);
        colors[i] = RandomHelper::RandomColor();
        outlineColors[i] = WHITE;
    }
}

void Application::Update() {

    // camera move
    camera.camera.zoom += ((float)GetMouseWheelMove() * camera.zoomSpeed);

    if (camera.camera.zoom > 30.0f) camera.camera.zoom = 30.0f;
    else if (camera.camera.zoom < 0.1f) camera.camera.zoom = 0.1f;

    if (IsKeyPressed(KEY_R))
    {
        camera.camera.zoom = defaultZoom;
        camera.camera.target = Vector2 { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        camera.camera.target.x += GetFrameTime() * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        camera.camera.target.x -= GetFrameTime() * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        camera.camera.target.y += GetFrameTime() * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_UP))
    {
        camera.camera.target.y -= GetFrameTime() * camera.linearSpeed;
    }
    // particle move

    float dx = 0.0f;
    float dy = 0.0f;
    float speed = 8.0f;

    if (IsKeyDown(KEY_D)) dx++;
    if (IsKeyDown(KEY_A)) dx--;
    if (IsKeyDown(KEY_W)) dy--;
    if (IsKeyDown(KEY_S)) dy++;

    if (dx || dy)
    {
        FlatVector direction = FlatMath::Normalize(FlatVector(dx, dy));
        FlatVector velocity = direction * speed * GetFrameTime();
        bodyVector[0]->Move(velocity);
    }

    for (int i = 0; i < bodyVector.size(); i++)
    {
        // FlatBody* body = bodyVector[i];
        bodyVector[i]->Rotate(PI / 2.0f * GetFrameTime());
        outlineColors[i] = WHITE;
    }

    for (int i = 0; i < bodyVector.size() - 1; i++)
    {
        FlatBody* bodyA = bodyVector[i];

        for (int j = i + 1; j < bodyVector.size(); j++)
        {
            FlatBody* bodyB = bodyVector[j];
            if (Collisions::IntersectPolygons(bodyA->GetTransformedVertices(), bodyB->GetTransformedVertices()))
            {
                
                outlineColors[i] = RED;
                outlineColors[j] = RED;
            }
          
#if false            
            FlatVector normal;
            float depth;
            if (Collisions::IntersectCircles(
                bodyA->GetPosition(), bodyA->Radius,
                bodyB->GetPosition(), bodyB->Radius,
                normal, depth))
            {
                bodyA->Move(-normal * depth / 2);
                bodyB->Move(normal * depth / 2);
            }
#endif            
        }
    }

    
}

void Application::Draw() {

    BeginMode2D(camera.camera);

    for (int i = 0; i < bodyCount; i++)
    {
        auto& body = bodyVector[i];
        Vector2 position = FlatConvertor::ToVector2(body->GetPosition());
        if (body->shapeType == ShapeType::Circle)
        {
            DrawCircle(position.x, position.y, body->Radius, colors[i]);
            
            DrawCircleLines(position.x, position.y, body->Radius, RAYWHITE);
        }
        else if (body->shapeType == ShapeType::Box)
        {
            auto vertices = body->GetTransformedVertices();
            FlatConvertor::ToVector2Array(vertices, vertexBuffer);
            
            DrawTriangleFan(&vertexBuffer[0], vertexBuffer.size(), colors[i]);
            GameDraw::DrawPolygonLines(vertices, outlineColors[i]);
            //DrawTriangleFan(vertexBuffer.data(), vertexBuffer.size(), outlineColors[i]);
        }

    }

    EndMode2D();

    DrawText(TextFormat("ZOOM : %d%%", int(camera.camera.zoom / defaultZoom * 100)), 50, GetScreenHeight() - 50 - 10, 20, YELLOW);
    DrawFPS(20, 20);
}

void Application::End() {
    delete[] colors;
    delete[] outlineColors;
    for (auto& body : bodyVector) {
        delete body;
    }
}