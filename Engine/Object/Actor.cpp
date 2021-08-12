#include "Actor.h"
#include "Graphics/Renderer.h"
#include "Math/Random.h"
#include "Math/MathUtil.h"
#include <algorithm>

namespace nc
{
	void Actor::Update(float dt)
	{
		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) {child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw(Renderer* renderer)
	{
		if (texture) renderer->Draw(texture, transform);
		std::for_each(children.begin(), children.end(), [renderer](auto& child) { child->Draw(renderer);  });
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