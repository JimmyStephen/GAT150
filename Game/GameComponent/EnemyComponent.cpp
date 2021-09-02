#include "EnemyComponent.h"
#include "Engine.h"

using namespace nc;

void EnemyComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&EnemyComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
}

void EnemyComponent::Update()
{
	Actor* player = owner->scene->FindActor("Player");
	if (player) {
		Vector2 direction = player->transform.position - owner->transform.position;
		Vector2 force = direction.Normalized() * speed;

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);
		physicsComponent->ApplyForce(force);
	}


}

bool EnemyComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool EnemyComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	return true;
}

void EnemyComponent::OnCollisionEnter(const nc::Event& event){
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Projectile") && istring_compare(owner->tag, "Enemy")) {
		auto actor2 = nc::ObjectFactory::Instance().Create<nc::Actor>("D_Duck");
        actor2->transform.position = owner->transform.position;
        owner->scene->AddActor(std::move(actor2));
	}
	if (istring_compare(actor->tag, "Ground")) {
		owner->destroy = true;
	}
}