#pragma once
#include <DirectXMath.h>
#include "entt.hpp"

struct Transform {
	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
};

struct Movable {
	//This is a marker struct to indicate that the entity can be moved
	//It s a temporary solution before scripting
};

struct TransformComponent {
	Transform transform;

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
};

class TransformSystem {

public:
	TransformSystem() = default;
	TransformSystem(const TransformSystem&) = delete;
	TransformSystem& operator=(const TransformSystem&) = delete;
	~TransformSystem() = default;
	void Update(entt::registry& registry, const Transform& transform);
};

inline bool PositionEquals(const Transform& a, const Transform& b) {
	return a.position.x == b.position.x && a.position.y == b.position.y && a.position.z == b.position.z;
}

inline bool RotationEquals(const Transform& a, const Transform& b) {
	return a.rotation.x == b.rotation.x && a.rotation.y == b.rotation.y && a.rotation.z == b.rotation.z;
}

inline bool ScaleEquals(const Transform& a, const Transform& b) {
	return a.scale.x == b.scale.x && a.scale.y == b.scale.y && a.scale.z == b.scale.z;
}

inline bool operator==(const Transform& lhs, const Transform& rhs) {
	return PositionEquals(lhs, rhs)
		&& RotationEquals(lhs, rhs)
		&& ScaleEquals(lhs, rhs);
}

inline bool operator!=(const Transform& lhs, const Transform& rhs) {
	return !(lhs == rhs);
}
