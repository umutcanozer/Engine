#include "BehaviourSystem.h"

BehaviourSystem::BehaviourSystem(entt::registry& registry) : m_registry(registry) 
{
}

void BehaviourSystem::Update(float deltaTime)
{
	auto view = m_registry.view<BehaviourComponent>();
	for (auto entity : view) {
		auto& behavior = view.get<BehaviourComponent>(entity);
		behavior.updateFunction(entity, m_registry, deltaTime);
	}
}
