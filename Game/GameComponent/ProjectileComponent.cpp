#include "ProjectileComponent.h"
#include "Engine.h"

using namespace nc;

ProjectileComponent::~ProjectileComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void ProjectileComponent::Create()
{
	Vector2 position = owner->scene->engine->Get<InputSystem>()->GetMousePosition();
	fireTo = Vector2{ position.x, position.y };

	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&ProjectileComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&ProjectileComponent::OnCollisionExit, this, std::placeholders::_1), owner);
	//owner->scene->engine->Get<AudioSystem>()->AddAudio("hit", "audio/coin.wav");
}

void ProjectileComponent::Update()
{
	lifetime -= owner->scene->engine->time.deltaTime;
	if(!owner->destroy) owner->destroy = (lifetime <= 0);
	if (fireTo.x - owner->transform.position.x <= 10 && fireTo.y - owner->transform.position.y <= 10 &&
		fireTo.x - owner->transform.position.x >= -10 && fireTo.y - owner->transform.position.y >= -10) owner->destroy = true;

	Vector2 direction = fireTo - owner->transform.position;
	Vector2 force = direction.Normalized() * speed;

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);
	physicsComponent->ApplyForce(direction);

}

void ProjectileComponent::OnCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Enemy")) {
		owner->destroy = true;		
		actor->destroy = true;

		Event event;
		event.name = "add_score";
		event.data = 100;
		owner->scene->engine->Get<EventSystem>()->Notify(event);
	}


}

void ProjectileComponent::OnCollisionExit(const nc::Event& event)
{

}


bool ProjectileComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool ProjectileComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, lifetime);
	return true;
}


