#pragma once
#include <Windows.h>

class IMessageHandler {
public:
    virtual void OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
    virtual ~IMessageHandler() = default;
};