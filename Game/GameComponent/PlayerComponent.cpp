#include "PlayerComponent.h"
#include "ProjectileComponent.h"
#include "Engine.h"

using namespace nc;

PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::readFile() {
	rapidjson::Document document;
	nc::json::Load("actors.txt", document);
	owner->scene->Read(document);
}

void PlayerComponent::Create()
{
	//readFile();
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter", std::bind(&PlayerComponent::OnCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit", std::bind(&PlayerComponent::OnCollisionExit, this, std::placeholders::_1), owner);

	owner->scene->engine->Get<AudioSystem>()->AddAudio("fire", "audio/Game_Gunshot.mp3");
	owner->scene->engine->Get<AudioSystem>()->AddAudio("jump", "audio/jump.mp3");
}

void PlayerComponent::Update()
{
	if (fireCoolDown <= 0) {
		if (owner->scene->engine->Get<InputSystem>()->GetButtonState((int)InputSystem::eMouseButton::Left) == InputSystem::eKeyState::Held)
		{
			Shoot();
		}
		fireCoolDown = fireRate;
	}
	fireCoolDown -= owner->scene->engine->time.deltaTime;

	Vector2 force = Vector2::zero;
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::Held)
	{
		force.x -= speed;
	}
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::Held)
	{
		force.x += speed;
	}
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == InputSystem::eKeyState::Held && contacts.size() != 0  )
	{
		force.y -= jump;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);
	physicsComponent->ApplyForce(force);

	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);

	if (physicsComponent->velocity.x > 1) { spriteAnimationComponent->SetSequence("walk_left"); }
	else if (physicsComponent->velocity.x < -1) { spriteAnimationComponent->SetSequence("walk_right"); }

}

void PlayerComponent::OnCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "enemy")) {
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
		owner->destroy = true;
	}
	if (istring_compare(actor->tag, "ground")) {
		contacts.push_back(actor);
	}
}

void PlayerComponent::OnCollisionExit(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);

	if (istring_compare(actor->tag, "ground")) {
		contacts.remove(actor);
	}
}

void PlayerComponent::Shoot()
{
	//HELP
	//auto actor = nc::ObjectFactory::Instance().Create<ProjectileComponent>("Bullet");
	//assert(actor);
	//actor->owner->transform.position = owner->transform.position;
	//actor->fireTo = nc::Vector2{ x,y };
	//owner->scene->AddActor(std::move(actor->owner));

	owner->scene->engine->Get<AudioSystem>()->PlayAudio("fire");
	auto actor = nc::ObjectFactory::Instance().Create<Actor>("Bullet");
	assert(actor);
	actor->transform.position = owner->transform.position;
	owner->scene->AddActor(std::move(actor));
}


bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	JSON_READ(value, jump);
	return true;
}

