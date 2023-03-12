#pragma once
#include "GameLogic\Game.h"


int main(int argc, char** argv)
{
	Game game;

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
	SetWindowState(FLAG_VSYNC_HINT);


	game.Setting();

	while (!WindowShouldClose()) {
		float time = GetFrameTime();
		game.Update(time);

		BeginDrawing();
		ClearBackground(SKYBLUE);

		game.Draw(time);

		EndDrawing();
	}

	game.End();

	return 0;
}




