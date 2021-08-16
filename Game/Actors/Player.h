#pragma once
#include "Object/Actor.h"

class Player : public nc::Actor
{
public:
	Player(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed, bool isEndless = false);
	virtual void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float immunityFrames = 0.75f;
	float immunityFrameTimer = 0;
	float fireRate = 0.25f;
	float fireTimer = 0;
	float speed{ 1000 };
	bool isEndless;
	nc::Vector2 velocity;

};