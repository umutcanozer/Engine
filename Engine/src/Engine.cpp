#include "Engine.h"
Engine::Engine()
{
	m_system = std::make_unique<System>();
	m_graphics = std::make_unique<Graphics>(m_system->GetWindow(), m_system->GetWindowWidth(), m_system->GetWindowHeight());
}

void Engine::Frame()
{
	m_graphics->BeginFrame(0.0f, 0.0f, 1.0f, 1.0f);
	m_graphics->EndFrame();
}

int Engine::Run()
{
    while (true)
	{
		if (const auto ecode = m_system->ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		if (m_system->GetKeyboard().KeyIsPressed(VK_ESCAPE))
		{
			MessageBox(nullptr, "ESC is pressed.", "Info", MB_OK);
			Sleep(300);
		}

		Frame();
	}
}