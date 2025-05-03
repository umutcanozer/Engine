#pragma once
#include <functional>
#include "entt.hpp"

struct BehaviourComponent
{
	std::function<void(entt::entity, entt::registry&, float)> updateFunction;
};
class BehaviourSystem
{
public:
	BehaviourSystem(entt::registry& registry);
	BehaviourSystem(const BehaviourSystem&) = delete;
	BehaviourSystem& operator=(const BehaviourSystem&) = delete;
	~BehaviourSystem() = default;

	void Update(float deltaTime);

private:
	entt::registry& m_registry;
};