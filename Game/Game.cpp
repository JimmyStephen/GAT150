#include "Game.h"
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
	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music2", 1, 2, true);

	texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Alex_Ship.png", engine->Get<nc::Renderer>());
	texture2 = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", engine->Get<nc::Renderer>());

	// get font from resource system
	int size = 72;
	font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/united.ttf", &size);

	// create font texture
	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("UwU", nc::Color{ 1, 1, 1 }));
	// add font texture to resource system
	engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);


	for (size_t i = 0; i < 25; i++) {
		nc::Transform transform{ {nc::RandomRange(0,800),nc::RandomRange(0,600)}, nc::RandomRange(0,360), 0.25f };
		std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
		scene->AddActor(std::move(actor));
	}

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
		//StartLevel();
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
		stateTimer -= .05;
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

	if (engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Held)
	{
		nc::Vector2 position = engine->Get<nc::InputSystem>()->GetMousePosition();
		std::cout << position.x << " " << position.y << std::endl;
		engine->Get<nc::ParticleSystem>()->Create({ position.x,position.y },1,1,texture2,1);
		engine->Get<nc::AudioSystem>()->PlayAudio("explosion", 1, 1, false);
		musicChannel.SetPitch(nc::RandomRange(.02f, 2.0f));
	}
}

void Game::Draw()
{
	switch (state)
	{
	case Game::eState::Title:
		//graphics.SetColor(nc::Color::red);
		//graphics.DrawString(310, 300 + std::sin(stateTimer * 6.0f) * 10.0f, "The Accident Wasn't Your Fault");
		//graphics.SetColor(nc::Color::green);
		//graphics.DrawString(340, 360, "Press [TAB] to Start!");
		//graphics.DrawString(250, 380, "Press [SPACE] to Play Cold War (Endless Mode!)");


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
		//if (scene->GetActors<Enemy>().size() == 0)
		//{
		//	destroyedAll = true;
		//	state = eState::GameOver;
		//}
		break;
	case Game::eState::GameEndless:
		GameEndless();
		break;
	case Game::eState::GameOver:
		//graphics.SetColor(nc::Color::red);
		//if (destroyedAll == true){
		//	graphics.DrawString(380, 300.0f + std::sin(stateTimer * 6.0f) * 10.0f, "You Won!");
		//}
		//else {
		//	graphics.DrawString(380, 300.0f + std::sin(stateTimer * 6.0f) * 10.0f, "Game Over");
		//}
		//graphics.SetColor(nc::Color::green);
		//graphics.DrawString(350, 350, "You Scored: ");
		//graphics.DrawString(435, 350, std::to_string(score).c_str());
		//graphics.DrawString(340, 370, "Press [TAB] to Start!");
		//graphics.DrawString(250, 390, "Press [SPACE] to Play Cold War (Endless Mode!)");
		break;
	default:
		break;
	}

	//graphics.SetColor(nc::Color::white);
	//graphics.DrawString(30, 30, "Score: ");
	//graphics.DrawString(75, 30, std::to_string(score).c_str());
	//graphics.DrawString(30, 50, "Lives: ");
	//graphics.DrawString(75, 50, std::to_string(lives).c_str());

	//if (state == eState::GameEndless) {
	//	graphics.DrawString(330, 30, "Gain Life In ");
	//	graphics.DrawString(420, 30, std::to_string(gainLife).c_str());
	//	graphics.DrawString(450, 30, "Points");
	//	graphics.DrawString(700, 30, "High Score");
	//	graphics.DrawString(700, 50, std::to_string(highScore).c_str());
	//}

	engine->Get<nc::Renderer>()->BeginFrame();

	nc::Transform t;
	t.position = { 30, 30 };
	engine->Get<nc::Renderer>()->Draw(textTexture, t);

	scene->Draw(engine->Get<nc::Renderer>());
	engine->Draw(engine->Get<nc::Renderer>());

	//make text follow mouse
	if (engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Held)
	{
		nc::Vector2 position = engine->Get<nc::InputSystem>()->GetMousePosition();
		position.x -= 65;
		position.y -= 25;
		engine->Get<nc::Renderer>()->Draw(textTexture, position);
	}

	engine->Get<nc::Renderer>()->EndFrame();


}

void Game::OnAddPoints(const nc::Event& event)
{
	//score += std::get<int>(event.data);
	//if (state == eState::GameEndless) {
	//	gainLife -= 100;
	//	if (gainLife == 0) {
	//		lives++;
	//		gainLife = 1000;
	//	}
	//	if (highScore < score) {
	//		nc::FileIO::WriteHighScore("Highscore.txt", score);
	//		highScore = score;
	//	}
	//}
}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives -= 1;
	if (lives == 0)state = eState::GameOver;
}

void Game::StartLevel() {
	////Ship
	//std::shared_ptr<nc::Shape> shapeShip = std::make_shared<nc::Shape>();
	//shapeShip->Load("ship.txt");

	////asteroid
	//std::shared_ptr<nc::Shape> shapeCustom3 = std::make_shared<nc::Shape>();
	//shapeCustom3->Load("asteroid.txt");

	//scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400, 300}, 0, 10 }, shapeShip, 300.0f));
	//for (size_t i = 0; i < 15; i++)
	//{
	//	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 5 }, shapeCustom3, 150.0f, false));
	//}
	//for (size_t i = 0; i < 10; i++)
	//{
	//	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 5 }, shapeCustom3, 50.0f, true));
	//}
}

void Game::StartEndlessLevel() {

	////flag
	//std::shared_ptr<nc::Shape> shapeCustom2 = std::make_shared<nc::Shape>();
	//shapeCustom2->Load("flag.txt");

	////hammer
	//std::shared_ptr<nc::Shape> hammer = std::make_shared<nc::Shape>();
	//hammer->Load("hammer.txt");

	//scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400, 300}, 0, 10 }, shapeCustom2, 300.0f));
	//scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 5 }, hammer, 100.0f, false));
}

void Game::GameEndless() {
	////russia
	//std::shared_ptr<nc::Shape> shapeCustom3 = std::make_shared<nc::Shape>();
	//shapeCustom3->Load("shape3.txt");

	////hammer
	//std::shared_ptr<nc::Shape> hammer = std::make_shared<nc::Shape>();
	//hammer->Load("hammer.txt");

	////sickle
	//std::shared_ptr<nc::Shape> sickle = std::make_shared<nc::Shape>();
	//sickle->Load("sickle.txt");

	//if (scene->GetActors<Enemy>().size() < (score / 100))
	//{
	//	if (score >= 1000 && score < 2000) {
	//		if (summonRanged) {
	//			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 5 }, sickle, 50.0f, true));
	//			summonRanged = false;
	//		}
	//		else {
	//			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 5 }, hammer, 150.0f, false));
	//			summonRanged = true;
	//		}
	//	}
	//	else if (score >= 2000) {
	//		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 5 }, shapeCustom3, 150.0f, true));
	//	}
	//	else {
	//		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 10.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 5 }, hammer, 100.0f, false));
	//	}
	//}
}