#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Math/Random.h"


void Game::Initialize()
{
	engine = std::make_unique<nc::Engine>(); //new Engine
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600);

	scene = std::make_unique<nc::Scene>(); // new Scene
	scene->engine = engine.get();

	nc::SetFilePath("../Resources");

	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("music", "audio/giorno_theme.mp3");
	engine->Get<nc::AudioSystem>()->AddAudio("music2", "audio/business-man.mp3");
	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music", 1, 2, true);

	texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Alex_Ship.png", engine->Get<nc::Renderer>());
	texture2 = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", engine->Get<nc::Renderer>());

	// get font from resource system
	int size = 16;
	font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/8514oem.fon", &size);

	// create font texture
//	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	// set font texture with font surface
//	textTexture->Create(font->CreateSurface("UwU", nc::Color{ 1, 1, 1 }));
	// add font texture to resource system
//	engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);

	//make actors
	//for (size_t i = 0; i < 25; i++) {
	//	nc::Transform transform{ {nc::RandomRange(0,800),nc::RandomRange(0,600)}, nc::RandomRange(0,360), 0.25f };
	//	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
	//	scene->AddActor(std::move(actor));
	//}

	//Game (Old)
	engine->Get<nc::AudioSystem>()->AddAudio("playerShoot", "audio/Player_Shoot.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("enemyShoot", "audio/Enemy_Shoot.wav");

	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
	//End Game
}

void Game::Shutdown()
{
	scene->RemoveAllActor();
	engine->Shutdown();
}

void Game::Update()
{
	float dt = engine->time.deltaTime;
	stateTimer += dt;

	switch (state)
	{
	case Game::eState::Title:
		highScore = nc::FileIO::LoadHighScore("Highscore.txt");
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_TAB) == nc::InputSystem::eKeyState::Pressed) {
			state = eState::StartGame;
		}
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed) {
			state = eState::StartEndless;
		}
		break;
	case Game::eState::StartGame:
		destroyedAll = false;
		score = 0;
		lives = 3;
		state = eState::StartLevel;
		break;
	case Game::eState::StartEndless:
		score = 0;
		lives = 3;
		gainLife = 1000;
		state = eState::StartEndlessLevel;
		break;
	case Game::eState::StartLevel:
		StartLevel();
		state = eState::Game;
		break;
	case Game::eState::StartEndlessLevel:
		StartEndlessLevel();
		state = eState::GameEndless;
		break;
	case Game::eState::Game:
		stateTimer = 2;
		break;
	case Game::eState::GameEndless:
		stateTimer = 2;
		break;
	case Game::eState::GameOver:
		stateTimer -= .05f;
		scene->RemoveAllActor();

		if (stateTimer <= 0) {
			if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_TAB) == nc::InputSystem::eKeyState::Pressed) {
				state = eState::StartGame;
			}
			if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed) {
				state = eState::StartEndless;
			}
		}
		break;
	default:
		break;
	}

	engine->Update();
	scene->Update(dt);

	quit = (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);

	//if (engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Held)
	//{
	//	nc::Vector2 position = engine->Get<nc::InputSystem>()->GetMousePosition();
	//	std::cout << position.x << " " << position.y << std::endl;
	//	engine->Get<nc::ParticleSystem>()->Create({ position.x,position.y },1,1,texture2,1);
	//	engine->Get<nc::AudioSystem>()->PlayAudio("explosion", 1, 1, false);
	//	musicChannel.SetPitch(nc::RandomRange(.02f, 2.0f));
	//}
}

void Game::Draw()
{
	// begin
	engine->Get<nc::Renderer>()->BeginFrame();

	switch (state)
	{
	case Game::eState::Title:
	{
		nc::Transform t;
		nc::Transform t2;
		nc::Transform t3;

		std::shared_ptr<nc::Texture> textTexture1;
		std::shared_ptr<nc::Texture> textTexture2;
		std::shared_ptr<nc::Texture> textTexture3;

		t.position = { 300, 250 };
		t2.position = { 250, 310 };
		t3.position = { 220, 330 };

		textTexture1 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture1->Create(font->CreateSurface("The Accident Wasn't Your Fault", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture1", textTexture1);

		textTexture2 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture2->Create(font->CreateSurface("Press [TAB] To Play Battle of The Alex's", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture2", textTexture2);

		textTexture3 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture3->Create(font->CreateSurface("Press [SPACE] to Play Cold War (Endless Mode!)", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture3", textTexture3);

		engine->Get<nc::Renderer>()->Draw(textTexture1, t);
		engine->Get<nc::Renderer>()->Draw(textTexture2, t2);
		engine->Get<nc::Renderer>()->Draw(textTexture3, t3);
	}
	break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartEndless:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::StartEndlessLevel:
		break;
	case Game::eState::Game:
		if (scene->GetActors<Enemy>().size() == 0)
		{
			destroyedAll = true;
			state = eState::GameOver;
		}
		break;
	case Game::eState::GameEndless:
		GameEndless();
		break;
	case Game::eState::GameOver:
	{
		nc::Transform l;
		std::shared_ptr<nc::Texture> textTexturel;
		l.position = { 380, 200 };
		if (destroyedAll == true) {
			textTexturel = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
			textTexturel->Create(font->CreateSurface("You Won", nc::Color{ 1, 1, 1 }));
			engine->Get<nc::ResourceSystem>()->Add("textTexturel", textTexturel);
		}
		else {
			textTexturel = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
			textTexturel->Create(font->CreateSurface("Game Over", nc::Color{ 1, 1, 1 }));
			engine->Get<nc::ResourceSystem>()->Add("textTexturel", textTexturel);
		}
		nc::Transform t;
		nc::Transform t2;
		nc::Transform t3;

		std::shared_ptr<nc::Texture> textTexture1;
		std::shared_ptr<nc::Texture> textTexture2;
		std::shared_ptr<nc::Texture> textTexture3;

		t.position = { 300, 250 };
		t2.position = { 250, 310 };
		t3.position = { 220, 330 };

		textTexture1 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture1->Create(font->CreateSurface("The Accident Wasn't Your Fault", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture1", textTexture1);

		textTexture2 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture2->Create(font->CreateSurface("Press [TAB] To Play Battle of The Alex's", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture2", textTexture2);

		textTexture3 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture3->Create(font->CreateSurface("Press [SPACE] to Play Cold War(Endless Mode!)", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture3", textTexture3);

		engine->Get<nc::Renderer>()->Draw(textTexturel, l);
		engine->Get<nc::Renderer>()->Draw(textTexture1, t);
		engine->Get<nc::Renderer>()->Draw(textTexture2, t2);
		engine->Get<nc::Renderer>()->Draw(textTexture3, t3);
	}
	break;
	default:
		break;
	}
	//Score, Lives, Highscore, exc
	{
		nc::Transform t;
		nc::Transform t2;

		std::shared_ptr<nc::Texture> textTexture1;
		std::shared_ptr<nc::Texture> textTexture2;

		t.position = { 30, 30 };
		t2.position = { 30, 50 };

		textTexture1 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture1->Create(font->CreateSurface("Score: " + std::to_string(score), nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture1", textTexture1);

		textTexture2 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		textTexture2->Create(font->CreateSurface("Lives: " + std::to_string(lives), nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("textTexture2", textTexture2);

		engine->Get<nc::Renderer>()->Draw(textTexture1, t);
		engine->Get<nc::Renderer>()->Draw(textTexture2, t2);

		if (state == eState::GameEndless) {
			nc::Transform t3;
			std::shared_ptr<nc::Texture> textTexture3;
			nc::Transform t4;
			std::shared_ptr<nc::Texture> textTexture4;
			
			t3.position = { 330, 30 };
			t4.position = { 600, 30 };

			textTexture3 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
			textTexture3->Create(font->CreateSurface("Gain Life In " + std::to_string(gainLife) + " Points", nc::Color{ 1, 1, 1 }));
			engine->Get<nc::ResourceSystem>()->Add("textTexture3", textTexture3);

			textTexture4 = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
			textTexture4->Create(font->CreateSurface("High Score: " + std::to_string(highScore), nc::Color{ 1, 1, 1 }));
			engine->Get<nc::ResourceSystem>()->Add("textTexture4", textTexture4);

			engine->Get<nc::Renderer>()->Draw(textTexture3, t3);
			engine->Get<nc::Renderer>()->Draw(textTexture4, t4);
		}
	}

	//end
	scene->Draw(engine->Get<nc::Renderer>());
	engine->Draw(engine->Get<nc::Renderer>());
	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data);
	if (state == eState::GameEndless) {
		gainLife -= 100;
		if (gainLife == 0) {
			lives++;
			gainLife = 1000;
		}
		if (highScore < score) {
			nc::FileIO::WriteHighScore("Highscore.txt", score);
			highScore = score;
		}
	}
}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives -= 1;
	if (lives == 0)state = eState::GameOver;
}

void Game::StartLevel() {
	////Ship
	std::shared_ptr<nc::Texture> spaceShip = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Alex_Ship.png", engine->Get<nc::Renderer>());

	//asteroid (replace "AleX_Ship.png" with astroid)
	std::shared_ptr<nc::Texture> astroid = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Alexis_Ship.png", engine->Get<nc::Renderer>());

	scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400, 300}, 0, .5f }, spaceShip, 300.0f));
	for (size_t i = 0; i < 25; i++)
	{
		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), .1f }, astroid, 150.0f, false));
	}
}

void Game::StartEndlessLevel() {

	////Flag
	std::shared_ptr<nc::Texture> spaceShip = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Flag.png", engine->Get<nc::Renderer>());

	//Enemy
	std::shared_ptr<nc::Texture> astroid = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Hammer.png", engine->Get<nc::Renderer>());

	scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400, 300}, 0, .08f }, spaceShip, 300.0f, true));
	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), .05f }, astroid, 150.0f, false));
}

void Game::GameEndless() {
	//mixed
	std::shared_ptr<nc::Texture> strong = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("HammerAndSickle.png", engine->Get<nc::Renderer>());
	//melee
	std::shared_ptr<nc::Texture> melee = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Hammer.png", engine->Get<nc::Renderer>());
	//ranged
	std::shared_ptr<nc::Texture> ranged = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Sickle.png", engine->Get<nc::Renderer>());


	if (scene->GetActors<Enemy>().size() < (score / 100))
	{
		if (score >= 1000 && score < 2000) {
			if (summonRanged) {
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), .05f }, ranged, 150.0f, true));
				summonRanged = false;
			}
			else {
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), .05f }, melee, 150.0f, false));

				summonRanged = true;
			}
		}
		else if (score >= 2000) {
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), .05f }, strong, 150.0f, true));
		}
		else {
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), .05f }, melee, 150.0f, false));
		}
	}
}