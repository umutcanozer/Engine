#include "System.h"

System::System()
{
	m_window = std::make_unique<Window>(800, 600, "Engine");
	m_window->SetMessageHandler(this);
}

void System::OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
	case WM_KILLFOCUS:
		m_keyboard.ClearState();
		break;
	case WM_KEYDOWN:
		m_keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
		m_keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		m_keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_DESTROY:
		// Will be refactored later if multiple windows are needed
		PostQuitMessage(0);
		break;
    }
}

std::optional<int> System::ProcessMessages() {
    MSG msg = {};

	ZeroMemory(&msg, sizeof(MSG));
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
    }
	return {};
}