#include "Scene.h"
#include "Engine.h"
#include "Actor.h"
#include <algorithm>

namespace nc
{
	void Scene::Update(float dt)
	{
		//add new actors
		actors.insert(actors.end(), std::make_move_iterator(newActors.begin()), std::make_move_iterator(newActors.end()));
		newActors.clear();

		// update actors
		std::for_each(actors.begin(), actors.end(), [dt](auto& actor) { actor->Update(dt); });

		// check collisions
		for (size_t i = 0; i < actors.size(); i++)
			for (size_t j = i + 1; j < actors.size(); j++)
			{
				if (actors[i]->destroy || actors[j]->destroy) continue;

				nc::Vector2 dir = actors[i]->transform.position - actors[j]->transform.position;
				float distance = dir.Length();
				if (distance < actors[i]->GetRadius() + actors[j]->GetRadius())
				{
					actors[i]->OnCollision(actors[j].get());
					actors[j]->OnCollision(actors[i].get());
				}
			}
		// destroy actor
		auto iter = actors.begin();
		while (iter != actors.end())
		{
			if ((*iter)->destroy)
			{
				iter = actors.erase(iter);
			}
			else {
				iter++;
			}
		}
	}
	
	void Scene::Draw(Renderer* renderer)
	{
		std::for_each(actors.begin(), actors.end(), [renderer](auto& actor) { actor->Draw(renderer); });
	}

	void Scene::AddActor(std::unique_ptr<Actor> actor)
	{
		actor.get()->scene = this;
		newActors.push_back(std::move(actor));
	}
	
	void Scene::RemoveActor(Actor* actor)
	{
		
	}

	void Scene::RemoveAllActor()
	{
		actors.clear();
	}
	
	bool Scene::Write(const rapidjson::Value& value) const
	{
		return false;
	}



	//SOMETHIgNG IS WronG HeERE (i think?)
	bool Scene::Read(const rapidjson::Value& value)
	{
		if (value.HasMember("actors") && value["actors"].IsArray()) {
			for (auto& actorValue : value["actors"].GetArray()) {
				std::string type;
				JSON_READ(actorValue, type);

				auto actor = ObjectFactory::Instance().Create<Actor>(type);
				if (actor) {
					actor->scene = this;
					actor->Read(actorValue);
					AddActor(std::move(actor));
				}
			}
		}
		return true;
	}
}