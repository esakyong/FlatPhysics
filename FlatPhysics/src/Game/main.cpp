
#include "Application.h"



int main(int argc, char** argv) {
	Application app;
	
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
	SetWindowState(FLAG_VSYNC_HINT);
	

	app.Setting();

	while (!WindowShouldClose()) {
		float time = GetFrameTime();
		app.Update(time);

		BeginDrawing();
		ClearBackground(SKYBLUE);
		
			app.Draw(time);

		EndDrawing();
	}

	app.End();
	return 0;
}

