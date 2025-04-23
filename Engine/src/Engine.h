#pragma once
#include <chrono>
#include "System/System.h"
#include "System/CameraController.h"
#include "Core/Graphics.h"
#include "ECSystem/Renderer.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

class Engine
{
public:
	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine();

	void Frame();
	int Run();
	
private:
	void InitImGui();
	void UpdateImGui();
private:
	std::unique_ptr<System> m_system;
	std::unique_ptr<Graphics> m_graphics;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<CBufferSystem> m_cbufferSystem;
	std::unique_ptr<MeshSystem> m_meshSystem;
	std::unique_ptr<CameraSystem> m_cameraSystem;
	std::unique_ptr<TransformSystem> m_transformSystem;
	std::unique_ptr<CameraController> m_cameraController;

	entt::registry m_registry;
};
