#include "PickupComponent.h"
#include "Engine.h"

using namespace nc;

void PickupComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PickupComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PickupComponent::OnCollisionExit, this, std::placeholders::_1), owner);
}

void PickupComponent::Update()
{

}

void PickupComponent::OnCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "Player")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("consumable");
		std::cout << "player coin colision"<< std::endl;
	}
	std::cout << "coin colision" << std::endl;
}

void PickupComponent::OnCollisionExit(const nc::Event& event)
{

}


bool PickupComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PickupComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	return true;
}

