#include "WindowClass.h"
#include "Window.h"

WindowClass::WindowClass() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = Window::WindowProc;
    wc.hInstance = GetInstance();
    wc.lpszClassName = m_className;
    RegisterClass(&wc);
}

WindowClass::~WindowClass() {
    UnregisterClass(m_className, GetInstance());
}

const char* WindowClass::GetName() {
    return m_className;
}

HINSTANCE WindowClass::GetInstance() {
    return GetModuleHandle(nullptr);
}
