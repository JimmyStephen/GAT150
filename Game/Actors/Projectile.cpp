#include "Projectile.h"
#include "Enemy.h"
#include "Player.h"
#include "Math/MathUtil.h"


void Projectile::Update(float dt)
{
	Actor::Update(dt);

	lifetime -= dt;
	destroy = (lifetime <= 0);

	transform.position += nc::Vector2::Rotate(nc::Vector2::down, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);
}

void Projectile::OnCollision(Actor* actor)
{
	if ((dynamic_cast<Enemy*>(actor) && tag != "enemy") || (dynamic_cast<Player*>(actor) && tag != "player"))
	{
		destroy = true;
	}
}