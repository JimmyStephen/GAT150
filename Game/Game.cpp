#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"
#include "GameComponent/ProjectileComponent.h"

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
    REGISTER_CLASS(ProjectileComponent);


    //create scene
    scene = std::make_unique<nc::Scene>(); //new Scene()
    scene->engine = engine.get();

    nc::SetFilePath("../Resources");
    engine->Get<nc::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));
    engine->Get<nc::AudioSystem>()->AddAudio("Title_Screen",    "audio/Game_Title.mp3");
    engine->Get<nc::AudioSystem>()->AddAudio("Game_Start",      "audio/Game_Start.mp3");
    engine->Get<nc::AudioSystem>()->AddAudio("Game_Failed",     "audio/Game_Failed.mp3");
    engine->Get<nc::AudioSystem>()->AddAudio("Game_Over",       "audio/Game_Over.mp3");
}

//void Game::spawnBat(int numToSpawn) {
//    for (int i = 0; i < numToSpawn; i++) {
//        auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Bat");
//        actor->transform.position = nc::Vector2{ nc::RandomRange(100,700), nc::RandomRange(100,200) };
//        scene->AddActor(std::move(actor));
//    }
//}

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

    switch (state)
    {
    case Game::eState::Reset:
        Reset();
        break;
    case Game::eState::Title:
        Title();
        break;
    case Game::eState::StartGame:
        StartGame();
        break;
    case Game::eState::StartLevel:
        break;
    case Game::eState::Level:
        Level();
        break;
    case Game::eState::PlayerDead:
        PlayerDead();
        break;
    case Game::eState::GameOver:
        GameOver();
        break;
    default:
        break;
    }

    //update score
    auto scoreActor = scene->FindActor("Score");
    if (scoreActor) {
        scoreActor->GetComponent<nc::TextComponent>()->SetText("Score: " + std::to_string(score));
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

void Game::Reset()
{
    score = 0;
    scene->RemoveAllActor();

    rapidjson::Document document;

    nc::json::Load("blocks.txt", document);
    scene->Read(document);

    nc::json::Load("background.txt", document);
    scene->Read(document);

    nc::Tilemap tilemap;
    tilemap.scene = scene.get();
    nc::json::Load("map.txt", document);
    tilemap.Read(document);
    tilemap.Create();

    
    nc::json::Load("title.txt", document);
    scene->Read(document);


    state = eState::Title;
}

void Game::Title()
{
    if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
    {
        auto title = scene->FindActor("Title");
        assert(title);
        title->destroy = true;
        auto controls = scene->FindActor("Controls");
        assert(controls);
        controls->destroy = true;
       
        state = eState::StartGame;
    }
}

void Game::StartGame()
{
    engine->Get<nc::AudioSystem>()->PlayAudio("Game_Start", .5f);
    rapidjson::Document document;
    nc::json::Load("actors.txt", document);
    scene->Read(document);

    state = eState::Level;
}


void Game::Level()
{
    spawnTimer -= engine->time.deltaTime;
    if (spawnTimer <= 0) {
        spawnTimer = nc::RandomRange(4, 6) - (score / 100);
        if (spawnTimer <= 0) spawnTimer = .75;
        auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Duck");
        actor->transform.position = nc::Vector2{ nc::RandomRange(25, 750), nc::RandomRange(50,75) };
        scene->AddActor(std::move(actor));
    }

    nc::Actor* player = scene->FindActor("Player");
    if (!player) {
        state = eState::PlayerDead;
    }
}

void Game::PlayerDead()
{
    scene->RemoveAllActor();

    rapidjson::Document document;
    nc::json::Load("blocks.txt", document);
    scene->Read(document);

    nc::json::Load("background.txt", document);
    scene->Read(document);

    nc::Tilemap tilemap;
    tilemap.scene = scene.get();
    nc::json::Load("map.txt", document);
    tilemap.Read(document);
    tilemap.Create();

    nc::json::Load("gameover.txt", document);
    scene->Read(document);

    state = eState::GameOver;
    engine->Get<nc::AudioSystem>()->PlayAudio("Game_Failed");
}

void Game::GameOver()
{
    //if (!engine->Get<nc::AudioChannel>()->IsPlaying()) {
    //    engine->Get<nc::AudioSystem>()->AddAudio("Game_Over", "audio/Game_Over.mp3");
    //}

    if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
    {
        state = eState::Reset;
    }
}

void Game::OnAddScore(const nc::Event& event)
{
    score += std::get<int>(event.data);
    
}
