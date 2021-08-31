#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

//int global = 10;

void Game::Initialize()
{
    //create engine
    engine = std::make_unique<nc::Engine>(); //new Engine()
    engine->Startup();
    engine->Get<nc::Renderer>()->Create("Gat150", 800, 600);

    //register classes
    REGISTER_CLASS(PlayerComponent);
    REGISTER_CLASS(EnemyComponent);
    REGISTER_CLASS(PickupComponent);


    //create scene
    scene = std::make_unique<nc::Scene>(); //new Scene()
    scene->engine = engine.get();

    nc::SetFilePath("../Resources");


    rapidjson::Document document;
    nc::json::Load("scene.txt", document);
    scene->Read(document);

    for (int i = 0; i < 1000; i++) {
        auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Coin");
        actor->transform.position = nc::Vector2{ nc::RandomRange(0,800), nc::RandomRange(400,500) };
        scene->AddActor(std::move(actor));
    }
}

void Game::Shutdown()
{
    scene->RemoveAllActor();
    engine->Shutdown();
}

void Game::Update()
{
    engine->Update();
    //update
    if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
    {
        quit = true;
    }
    scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
    engine->Get<nc::Renderer>()->BeginFrame();

    engine->Draw(engine->Get<nc::Renderer>());
    scene->Draw(engine->Get<nc::Renderer>());

    engine->Get<nc::Renderer>()->EndFrame();
}