#pragma once
#include <Windows.h>

class WindowClass {
public:
    static WindowClass& Get() {
        static WindowClass instance;
        return instance;
    }
    static const char* GetName();
    static HINSTANCE GetInstance();

private:
    WindowClass();
    ~WindowClass();

    WindowClass(const WindowClass&) = delete;
    WindowClass& operator=(const WindowClass&) = delete;

    static inline constexpr const char* m_className = "MyWindowClass";
    HINSTANCE m_hInstance;
};
