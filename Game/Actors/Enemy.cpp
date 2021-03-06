#include "enemy.h"
#include "player.h"
#include "Math/MathUtil.h"
#include "Projectile.h"
#include "Engine.h"

void Enemy::OnCollision(Actor* actor)
{
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "player")
	{
		destroy = true;

		nc::Event event;

		event.name = "AddPoints";
		event.data = 100;
		scene->engine->Get<nc::EventSystem>()->Notify(event);

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");
	}
}

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>())
	{
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation);

		float turnAngle = nc::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (3 * dt);

		float angle = nc::Vector2::Angle(forward, direction.Normalized());

		//transform.rotation += nc::QuarterPi;
		transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
		transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
		transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

		transform.Update();
		fireTimer -= dt;

		if (doesShoot) {
			if (fireTimer <= 0 and angle <= nc::DegToRad(15))
			{
				fireTimer = fireRate;
				std::shared_ptr<nc::Texture> texture2 = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("EnemyStar.png", scene->engine->Get<nc::Renderer>());

				nc::Transform t = transform;
				t.scale = .25f;
				t.rotation += nc::HalfPi;

				std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, texture2, 300.0f);
				projectile->tag = "enemy";
				//projectile->tag = "player";
				scene->AddActor(std::move(projectile));
				
				scene->engine->Get<nc::AudioSystem>()->PlayAudio("enemyShoot");
			}
		}
	}
}