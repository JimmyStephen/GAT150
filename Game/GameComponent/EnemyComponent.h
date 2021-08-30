#pragma once
#include "Component/Component.h"


class EnemyComponent : public nc::Component
{
public:
	virtual bool Write(const rapidjson::Value& value) const override;
	virtual bool Read(const rapidjson::Value& value) override;
	virtual void Update() override;
	std::unique_ptr<Object> Clone() const { return std::make_unique<EnemyComponent>(*this); };

public:
	float speed{ 0 };
};




