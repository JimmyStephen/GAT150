#pragma once
#include "Engine.h"


class Game
{
public:
	enum class eState
	{
		Reset,
		Title,
		StartGame,
		StartLevel,
		Level,
		PlayerDead,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();
	void OnPlayerDead(const nc::Event& event);
	void spawnBat(int numToSpawn);

	bool IsQuit() { return quit; }
public:
	std::unique_ptr<nc::Engine> engine;
	std::unique_ptr<nc::Scene> scene;

private:
	void Reset();
	void Title();
	void StartGame();
	void StartLevel();
	void Level();
	void PlayerDead();
	void GameOver();

	void OnAddScore(const nc::Event& event);

	eState state = eState::Reset;
	int score = 0;
	float stateTimer = 0;
	float spawnTimer = 0;

	bool destroyedAll = false;
	bool quit = false;
	void (Game::* stateFunction)(float) = nullptr;

	size_t highScore = 0;
	size_t lives = 0;
	size_t gainLife = 1000;

	nc::AudioChannel musicChannel;

};