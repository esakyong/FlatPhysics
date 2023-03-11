#include "Application.h"




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
    SetTargetFPS(60);
    SetWindowPosition(10, 40);

    CameraExtents CameraExtent = camera.GetExtents();
    
    float padding = (CameraExtent.right - CameraExtent.left) * 0.1f;
    
    FlatBody* groundBody = new FlatBody();

    if (!FlatBody::CreateBoxBody(CameraExtent.right - CameraExtent.left - padding * 2, 30.0f, 1.0f, 
        true, 0.5f, *groundBody, errorMessage))
    {
        throw std::invalid_argument(errorMessage);
    }
    groundBody->MoveTo({ 0, 100.0f });
    world.AddBody(groundBody);
    entityVector.push_back(new FlatEntity(groundBody, DARKGREEN));
    
    

    FlatBody* ledgeBody1 = new FlatBody();
    if (!FlatBody::CreateBoxBody(200.0f, 20.0f, 1.0f, 
        true, 0.5f, *ledgeBody1, errorMessage))
    {
        throw std::invalid_argument(errorMessage);
    }
    ledgeBody1->MoveTo({ -100.0f, -30.0f });
    ledgeBody1->Rotate(PI / 10.0f);
    world.AddBody(ledgeBody1);
    entityVector.push_back(new FlatEntity(ledgeBody1, DARKGRAY));
    
    

    FlatBody* ledgeBody2 = new FlatBody();
    if (!FlatBody::CreateBoxBody(150.0f, 20.0f, 1.0f, 
        true, 0.5f, *ledgeBody2, errorMessage))
    {
        throw std::invalid_argument(errorMessage);
    }
    ledgeBody2->MoveTo({ 100.0f, -100.0f });
    ledgeBody2->Rotate(-PI / 10.0f);
    world.AddBody(ledgeBody2);
    entityVector.push_back(new FlatEntity(ledgeBody2, BROWN));
    
   
}

void Application::Update(float deltaTime) {

    // camera move
    camera.camera.zoom += ((float)GetMouseWheelMove() * camera.zoomSpeed);

    if (camera.camera.zoom > 30.0f) camera.camera.zoom = 30.0f;
    else if (camera.camera.zoom < 0.01f) camera.camera.zoom = 0.01f;

    // add box body
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        float width = RandomHelper::RandomFloat(20.0f, 30.0f);
        float height = RandomHelper::RandomFloat(20.0f, 30.0f);

        FlatVector mouseWorldPosition = 
            FlatConverter::ToFlatVector(GetScreenToWorld2D(GetMousePosition(), camera.camera));

        entityVector.push_back(new FlatEntity(world, width, height, false, mouseWorldPosition));
    }

    // add circle body
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        float radius = RandomHelper::RandomFloat(12.5f, 17.5f);

        FlatVector mouseWorldPosition =
            FlatConverter::ToFlatVector(GetScreenToWorld2D(GetMousePosition(), camera.camera));

        entityVector.push_back(new FlatEntity(world, radius, false, mouseWorldPosition));
        
    }

    if (IsKeyPressed(KEY_GRAVE))
    {
        std::cout << "BodyCount: " << world.BodyCount() << "\n\n";
    }

    if (IsKeyPressed(KEY_T))
    {
        camera.camera.zoom = defaultZoom;
        camera.camera.target = Vector2 { 0, 0 };
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
    

    
    world.Step(deltaTime, 20);

    CameraExtents extents = camera.GetExtents();
    float viewBottom = extents.top;

    entityRemovalVector.clear();

    for (int i = 0; i < world.BodyCount(); i++)
    {
        FlatEntity* entity = entityVector[i];
        FlatBody* body = entity->GetBody();
        
        if (!world.GetBody(i, body))
        {
            throw std::out_of_range("Body is out of range");
        }
        
        if (body->IsStatic)
        {
            continue;
        }

        FlatAABB box = body->GetAABB();

        if (box.Max.y > viewBottom)
        {
            entityRemovalVector.push_back(entity);
        }
    }

    for (int i = 0; i < entityRemovalVector.size(); i++)
    {
        FlatEntity* entity = entityRemovalVector[i];
        world.RemoveBody(entity->GetBody());
        entityVector.erase(remove(entityVector.begin(), entityVector.end(), entity), entityVector.end());
    }
    
}

void Application::Draw(float deltaTime) {

    BeginMode2D(camera.camera);

    for (int i = 0; i < entityVector.size(); i++)
    {
        entityVector[i]->Draw();
    }

    EndMode2D();

    DrawText(TextFormat("StepTime : %.4f", GetFrameTime()), 20, GetScreenHeight() - 30 - 10 - 20 - 20, 20, YELLOW);
    DrawText(TextFormat("BodyCount : %d", world.BodyCount()), 20, GetScreenHeight() - 30 - 10 - 20, 20, YELLOW);
    DrawText(TextFormat("Zoom : %d %%", int(camera.camera.zoom / defaultZoom * 100)), 20, GetScreenHeight() - 30 - 10, 20, YELLOW);
    DrawFPS(20, 20);
}

void Application::End() {
   
}

