#pragma once
#include <memory>
#include <optional>
#include "Window.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "IMessageHandler.h"

class System : public IMessageHandler
{
public:
	System();
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	~System() = default;

	HWND GetWindow() const { return m_window->GetHWnd(); }
	int GetWindowWidth() const { return m_window->GetWidth(); }
	int GetWindowHeight() const { return m_window->GetHeight(); }
	void OnMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	std::optional<int> ProcessMessages();

	Keyboard& GetKeyboard() { return m_keyboard; }
	Mouse& GetMouse() { return m_mouse; }

private:
	std::unique_ptr<Window> m_window;
	Keyboard m_keyboard;
	Mouse m_mouse;
};

