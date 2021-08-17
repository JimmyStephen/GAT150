#pragma once
#include "Object/Actor.h"

class Enemy : public nc::Actor
{
public:
	//Enemy(const nc::Transform transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed } {};
	Enemy(const nc::Transform transform, std::shared_ptr<nc::Texture> texture, float speed, bool doesFire) : nc::Actor{ transform }, speed{ speed }, doesShoot{ doesFire } {};
	void OnCollision(Actor* actor) override;

	virtual void Update(float dt) override;
private:

	bool doesShoot = false;
	float speed{ 300 };
	float fireTimer = 0;;
	float fireRate = .75f;
};