#pragma once
#include "System/System.h"
#include "Core/Graphics.h"
#include "ECSystem/Renderer.h"

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
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<CBufferSystem> m_cbufferSystem;
	std::unique_ptr<MeshSystem> m_meshSystem;

	entt::registry m_registry;
};
