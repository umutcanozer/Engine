#pragma once
#include <string>
#include "IMessageHandler.h"
#include "WindowClass.h"

class Window {
public:
    Window(int width, int height, const std::string& title);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
    ~Window() = default;

    HWND GetHWnd() const;
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void SetMessageHandler(IMessageHandler* handler);

private:
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    
private:
    HWND m_hWnd;
    HINSTANCE m_hInstance;

    IMessageHandler* m_pHandler = nullptr;

    int m_width;
    int m_height;
};
