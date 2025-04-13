#pragma once
#include "System.h"

class Engine
{
public:
	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	~Engine() = default;


	int Run();

private:
	std::unique_ptr<System> m_system;
};

