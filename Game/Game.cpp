#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Math/Random.h"
#include "Component/PhysicsComponent.h"


void Game::Initialize()
{
	engine = std::make_unique<nc::Engine>(); //new Engine
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600);

	scene = std::make_unique<nc::Scene>(); // new Scene
	scene->engine = engine.get();

	nc::SetFilePath("../Resources");

	std::unique_ptr<nc::Actor> actor = std::make_unique <nc::Actor>(nc::Transform{ {400, 300} });
	{
		std::unique_ptr<nc::SpriteComponent> component = std::make_unique <nc::SpriteComponent>();
		component->texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine->Get<nc::Renderer>());
		actor->AddComponent(std::move(component));
	}
	{
		std::unique_ptr<nc::PhysicsComponent> component = std::make_unique <nc::PhysicsComponent>();
		component->ApplyForce(nc::Vector2::right * 200);
		actor->AddComponent(std::move(component));
	}
	scene->AddActor(std::move(actor));
}

void Game::Shutdown()
{
	scene->RemoveAllActor();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();

	quit = (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	// begin
	engine->Get<nc::Renderer>()->BeginFrame();

	
	//end
	scene->Draw(engine->Get<nc::Renderer>());
	engine->Draw(engine->Get<nc::Renderer>());
	engine->Get<nc::Renderer>()->EndFrame();
}