#include "TransformSystem.h"

/*
*  Example update, you can replace this with actual logic like scripting etc.
*/

void TransformSystem::Update(entt::registry& registry, const Transform& newTransform)
{
	auto view = registry.view<TransformComponent>();
	for (auto entity : view) {
		auto& transformComponent = view.get<TransformComponent>(entity);

		if (!RotationEquals(transformComponent.transform, newTransform)) {
			transformComponent.transform.rotation.x = newTransform.rotation.x; 
			transformComponent.transform.rotation.y = newTransform.rotation.y;
			transformComponent.transform.rotation.z = newTransform.rotation.z;
		}


		if (!PositionEquals(transformComponent.transform, newTransform)) {
			transformComponent.transform.position.x += newTransform.position.x;
			transformComponent.transform.position.y = newTransform.position.y;
			transformComponent.transform.position.z += newTransform.position.z;
		}

		if (!ScaleEquals(transformComponent.transform, newTransform)) {
			transformComponent.transform.scale.x = newTransform.scale.x; 
			transformComponent.transform.scale.y = newTransform.scale.y;
			transformComponent.transform.scale.z = newTransform.scale.z;
		}
	}
}
