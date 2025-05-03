#pragma once
#include "Core/Scene.h"
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

	void Init();
	void Frame();
	int Run();
	
private:
	void InitImGui();
	void UpdateImGui();
private:
	std::unique_ptr<System> m_system;
	std::unique_ptr<Graphics> m_graphics;

	std::unique_ptr<Scene> m_scene;
};
