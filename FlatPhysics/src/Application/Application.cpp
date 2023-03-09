#include "Application.h"


#include "RandomHelper.h"

#include <stdexcept>

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
    
    float padding = (CameraExtent.right - CameraExtent.left) * 0.1f;
    

    if (!FlatBody::CreatBoxBody(CameraExtent.right - CameraExtent.left - padding * 2, 30.0f, { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 100.0f },
        1.0f, true, 0.5f, *goundBody, errorMessage))
    {
        throw std::invalid_argument(errorMessage);
    }
    
    world.AddBody(goundBody);

    colors.push_back(DARKGREEN);
    outlineColors.push_back(WHITE);

 
}

void Application::Update(float deltaTime) {

    // camera move
    camera.camera.zoom += ((float)GetMouseWheelMove() * camera.zoomSpeed);

    if (camera.camera.zoom > 30.0f) camera.camera.zoom = 30.0f;
    else if (camera.camera.zoom < 0.01f) camera.camera.zoom = 0.01f;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        float width = RandomHelper::RandomFloat(5, 20);
        float height = RandomHelper::RandomFloat(5, 20);
        FlatBody* body = new FlatBody();

        FlatVector worldMousePosition = FlatConvertor::ToFlatVector(GetScreenToWorld2D(GetMousePosition(), camera.camera));
        if (!FlatBody::CreatBoxBody(width, height, worldMousePosition, 2.0f, false, 0.5f, *body, errorMessage))
        {

        }
    }

    if (IsKeyPressed(KEY_T))
    {
        camera.camera.zoom = defaultZoom;
        camera.camera.target = Vector2 { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        camera.camera.target.x += deltaTime * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        camera.camera.target.x -= deltaTime * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        camera.camera.target.y += deltaTime * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_UP))
    {
        camera.camera.target.y -= deltaTime * camera.linearSpeed;
    }
    // particle move
#if false
    float dx = 0.0f;
    float dy = 0.0f;
    float forceMagnitude = 240000.0f;

    if (IsKeyDown(KEY_D)) dx++;
    if (IsKeyDown(KEY_A)) dx--;
    if (IsKeyDown(KEY_W)) dy--;
    if (IsKeyDown(KEY_S)) dy++;

    FlatBody* body;
    if (!world.GetBody(0, body))
    {
        // throw Exception("Could not find the body at the specified index.");
        std::cout << "Could not find the body at the specified index." << "\n";
        exit(1);
    }

    if (dx || dy)
    {
        FlatVector forcedirection = FlatMath::Normalize(FlatVector(dx, dy));
        FlatVector force = forcedirection * forceMagnitude;
        body->AddForce(force);
    }

    if (IsKeyDown(KEY_R))
    {
        body->Rotate(PI / 2.0f * deltaTime);
    }
#endif
    
    
    world.Step(deltaTime);
    
}

void Application::Draw(float deltaTime) {

    BeginMode2D(camera.camera);

    for (int i = 0; i < world.BodyCount(); i++)
    {
        FlatBody* body;
        if (!world.GetBody(i, body))
        {
            // throw Exception("Could not find a body at the specified index.");
            std::cout << "Could not find the body at the specified index." << "\n";
            exit(1);
        }

        Vector2 position = FlatConvertor::ToVector2(body->GetPosition());
        if (body->shapeType == ShapeType::Circle)
        {
            DrawCircleV(position, body->Radius, colors[i]);
            
            DrawRing(position, body->Radius * 0.9f, body->Radius, 0, 360, 100, outlineColors[i]);
        }
        else if (body->shapeType == ShapeType::Box)
        {
            std::vector<FlatVector> vertices = body->GetTransformedVertices();
            FlatConvertor::ToVector2Array(vertices, vertexBuffer);
            
            DrawTriangleFan(vertexBuffer.data(), vertexBuffer.size(), colors[i]);
            //GameDraw::DrawPolygonLines(vertexBuffer, 3, outlineColors[i]);
            
        }

    }

    EndMode2D();

    DrawText(TextFormat("ZOOM : %d %%", int(camera.camera.zoom / defaultZoom * 100)), 50, GetScreenHeight() - 50 - 10, 20, YELLOW);
    DrawFPS(20, 20);
}

void Application::End() {
   
}

void Application::WrapScreen()
{
    CameraExtents extent;
    extent = camera.GetExtents();

    float viewWidth = extent.right - extent.left;
    float viewHeight = extent.top - extent.bottom;

    for (int i = 0; i < world.BodyCount(); i++)
    {
        FlatBody* body;
        if (!world.GetBody(i, body))
        {
            std::cout << "error" << "\n";
            exit(-1);
        }
        if (body->GetPosition().x < extent.left) { body->MoveTo(body->GetPosition() + FlatVector(viewWidth, 0.0f)); }
        if (body->GetPosition().x > extent.right) { body->MoveTo(body->GetPosition() - FlatVector(viewWidth, 0.0f)); }
        if (body->GetPosition().y < extent.bottom) { body->MoveTo(body->GetPosition() + FlatVector(0.0f, viewHeight)); }
        if (body->GetPosition().y > extent.top) { body->MoveTo(body->GetPosition() - FlatVector(0.0f, viewHeight)); }
    }
}