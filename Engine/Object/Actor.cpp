#include "Actor.h"
#include "Graphics/Renderer.h"
#include "Math/Random.h"
#include "Math/MathUtil.h"
#include <algorithm>

namespace nc
{
	void Actor::Update(float dt)
	{
		transform.rotation		-= 2 * 180 * dt;
		transform.position.x	+= nc::RandomRangeInt(-100,100) * (100.0f * dt);
		transform.position.y	+= nc::RandomRangeInt(-100,100) * (100.0f * dt);
		transform.position.x	= nc::Wrap(transform.position.x, 0.0f, 800.0f);
		transform.position.y	= nc::Wrap(transform.position.y, 0.0f, 600.0f);

		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) {child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw(Renderer* renderer)
	{
		renderer->Draw(texture, transform);
	}
	
	float Actor::GetRadius()
	{
		return (texture) ? texture->GetSize().Length() * .5f : 0.0f;
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;
		children.push_back(std::move(actor));
	}



}