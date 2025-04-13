#pragma once
#include "System/System.h"
#include "Core/Graphics.h"

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
};
