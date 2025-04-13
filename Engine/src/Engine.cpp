#include "Engine.h"

Engine::Engine() : m_system(std::make_unique<System>()){}

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
	}
}