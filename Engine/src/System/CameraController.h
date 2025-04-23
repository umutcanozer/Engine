#pragma once
#include "System.h"
#include "ECSystem/TransformSystem.h"
#include "ECSystem/CameraSystem.h"
#include "entt.hpp"

class CameraController
{
public:
	CameraController(System& system, entt::registry& registry);
	CameraController(const CameraController&) = delete;
	CameraController& operator=(const CameraController&) = delete;
	~CameraController() = default;
	void Update(float deltaTime);

private:
	System& m_system;
	entt::registry& m_registry;
	const float m_moveSpeed = 3.5f;
	const float m_rotationSpeed = 5.f;
};

