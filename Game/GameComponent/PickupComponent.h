#pragma once
#include "Component/Component.h"
#include "Framework/EventSystem.h"
#include <list>

class PickupComponent : public nc::Component
{
public:
	void Create() override;
	virtual void Update() override;

	virtual ~PickupComponent();
	std::unique_ptr<Object> Clone() const { return std::make_unique<PickupComponent>(*this); };

	virtual void OnCollisionEnter(const nc::Event& event);
	virtual void OnCollisionExit(const nc::Event& event);

	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;

public:
	float speed{ 0 };

private:
	std::list<nc::Actor*> contacts;
};