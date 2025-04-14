#pragma once
#include "System/System.h"
#include "Core/Graphics.h"
#include "ECSystem/RenderSystem.h"
#include "ECSystem/CBufferSystem.h"
#include "entt.hpp"

class Engine
{
public:
	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;

	void Frame();
	int Run();
private:
	std::unique_ptr<System> m_system;
	std::unique_ptr<Graphics> m_graphics;
	std::unique_ptr<RenderSystem> m_renderSystem;
	std::unique_ptr<CBufferSystem> m_cbufferSystem;

	entt::registry m_registry;
	entt::entity cube;
};
