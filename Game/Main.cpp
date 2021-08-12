#include "Engine.h"
#include <iostream>
#include <SDL_Image.h>
#include <SDL.h>
#include <algorithm>
#include <cassert>

int main(int, char**)
{
	nc::Engine engine;
	engine.Startup();
	engine.Get<nc::Renderer>()->Create("GAT150", 800, 600);

	nc::Scene scene;
	scene.engine = &engine;

	nc::SetFilePath("../Resources");

	engine.Get<nc::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine.Get<nc::AudioSystem>()->AddAudio("music", "audio/giorno_theme.mp3");
	engine.Get<nc::AudioSystem>()->AddAudio("music2", "audio/business-man.mp3");
	nc::AudioChannel channel = engine.Get<nc::AudioSystem>()->PlayAudio("music2", 1, 2, true);
	
	std::shared_ptr<nc::Texture> texture = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("One_Alex_Churro.png", engine.Get<nc::Renderer>());
	std::shared_ptr<nc::Texture> texture2 = engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", engine.Get<nc::Renderer>());

	// get font from resource system
	int size = 72;
	std::shared_ptr<nc::Font> font = engine.Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/united.ttf", &size);

	// create font texture
	std::shared_ptr<nc::Texture> textTexture = std::make_shared<nc::Texture>(engine.Get<nc::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("UwU", nc::Color{ 1, 1, 1 }));
	// add font texture to resource system
	engine.Get<nc::ResourceSystem>()->Add("textTexture", textTexture);


	//for (size_t i = 0; i < 250; i++) {
	//	nc::Transform transform{ {nc::RandomRange(0,800),nc::RandomRange(0,600)}, nc::RandomRange(0,360), 0.25f};
	//	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
	//	scene.AddActor(std::move(actor));
	//}

	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 3;

	while (!quit)
	{
		engine.Update();
		quit = (engine.Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);
		scene.Update(engine.time.deltaTime);

		engine.Get<nc::Renderer>()->BeginFrame();

		nc::Transform t;
		t.position = { 30, 30 };
		engine.Get<nc::Renderer>()->Draw(textTexture, t);

		scene.Draw(engine.Get<nc::Renderer>());
		engine.Draw(engine.Get<nc::Renderer>());

		engine.Get<nc::Renderer>()->EndFrame();


		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		if (engine.Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Held)
		{
			nc::Vector2 position = engine.Get<nc::InputSystem>()->GetMousePosition();
			std::cout << position.x << " " << position.y << std::endl;
			engine.Get<nc::ParticleSystem>()->Create({ position.x,position.y },1,1,texture2,1);
			engine.Get<nc::AudioSystem>()->PlayAudio("explosion", 1, 1, false);
			channel.SetPitch(nc::RandomRange(.02f, 2.0f));
		}
	}

	SDL_Quit();

	return 0;
}
