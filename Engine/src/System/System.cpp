#include "System.h"

System::System()
{
	m_window = std::make_unique<Window>(1200, 700, "Engine");
	m_window->SetMessageHandler(this);
	m_mouse.EnableRaw();
}

void System::OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
	case WM_KILLFOCUS:
		m_keyboard.ClearState();
		ShowCursor(TRUE);
		break;
	case WM_SETFOCUS:
		ShowCursor(FALSE);
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
	case WM_MOUSEMOVE:
		POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnMouseMove(pt.x, pt.y);
		break;
	case WM_INPUT:
		if (m_mouse.RawEnabled()) {
			UINT size = 0;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
			std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(size);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &size, sizeof(RAWINPUTHEADER)) == size)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
				if (raw->header.dwType == RIM_TYPEMOUSE && (raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0)) {
					m_mouse.OnRawDelta(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}
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