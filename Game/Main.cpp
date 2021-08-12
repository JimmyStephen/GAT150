#include "Game.h"

int main(int, char**)
{
	Game game;
	game.Initialize();

	SDL_Event event;
	bool quit = false;
	while (!quit && !game.IsQuit())
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		game.Update();
		game.Draw();
	}

	SDL_Quit();

	return 0;
}
