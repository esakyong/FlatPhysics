#include "Common.h"
#include "Application.h"



int main(int argc, char** argv) {

	
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
	SetWindowState(FLAG_VSYNC_HINT);
	SetTargetFPS(60);

	app.Setting();

	while (!WindowShouldClose()) {

		app.Update();

		BeginDrawing();
		ClearBackground(SKYBLUE);

			app.Draw();

		EndDrawing();
	}

	app.End();
	return 0;
}

