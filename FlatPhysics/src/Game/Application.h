
#include "GameLogic\Game.h"

class Application
{
public:

	void Run()
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

	}

};


