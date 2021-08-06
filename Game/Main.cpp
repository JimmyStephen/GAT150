#include "Engine.h"
#include <iostream>
#include <SDL_Image.h>
#include <SDL.h>

int main(int, char**)
{

	nc::Engine engine;
	engine.Startup();
	engine.Get<nc::Renderer>()->Create("GAT150", 800, 600);

	nc::Scene scene;
	scene.engine = &engine;

	nc::SetFilePath("../Resources");

	std::shared_ptr<nc::Texture> texture = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("One_Alex_Churro.png", engine.Get<nc::Renderer>());

	for (size_t i = 0; i < 250; i++) {
		nc::Transform transform{ {nc::RandomRange(0,800),nc::RandomRange(0,600)}, nc::RandomRange(0,360), 0.25f};
		std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
		scene.AddActor(std::move(actor));
	}

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		engine.Update(0);
		scene.Update(0);

		engine.Get<nc::Renderer>()->BeginFrame();

		scene.Draw(engine.Get<nc::Renderer>());

		engine.Get<nc::Renderer>()->EndFrame();
	}

	SDL_Quit();

	return 0;
}
