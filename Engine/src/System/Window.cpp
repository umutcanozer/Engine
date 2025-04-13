#include "Window.h"

Window::Window(int width, int height, const std::string& title)
	: m_width(width), m_height(height)
{
    m_hInstance = WindowClass::Get().GetInstance();
    m_hWnd = CreateWindowEx(
        0, WindowClass::Get().GetName(), title.c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        width, height, nullptr, nullptr, m_hInstance, this
    );

    ShowWindow(m_hWnd, SW_SHOW);
}

HWND Window::GetHWnd() const {
    return m_hWnd;
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));

        pWindow->m_hWnd = hWnd;
    }

    Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (pWindow)
    {
        return pWindow->HandleMessage(msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (m_pHandler) {
        m_pHandler->OnMessage(m_hWnd, msg, wParam, lParam);
    }

    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(m_hWnd);
        return 0;
    }

    return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

void Window::SetMessageHandler(IMessageHandler* handler)
{
    m_pHandler = handler;
}
