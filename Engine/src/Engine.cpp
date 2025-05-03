#include "Engine.h"	

using namespace DirectX;

Engine::Engine()
{
	m_system = std::make_unique<System>();
	m_graphics = std::make_unique<Graphics>(m_system->GetWindow(), m_system->GetWindowWidth(), m_system->GetWindowHeight());
	m_scene = std::make_unique<Scene>(*m_graphics, *m_system);
}

void Engine::Init()
{
	InitImGui();
	m_scene->Init();
}

void Engine::Frame()
{
	m_graphics->BeginFrame(0.2f, 0.4f, 1.0f, 1.0f);
	m_scene->Update();
	UpdateImGui();
	m_graphics->EndFrame();
}


int Engine::Run()
{
	while (true)
	{
		if (const auto ecode = m_system->ProcessMessages())
		{
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

void Engine::InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(m_system->GetWindow());
	ImGui_ImplDX11_Init(m_graphics->GetDevice(), m_graphics->GetContext());
	ImGui::StyleColorsDark();
}

void Engine::UpdateImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGui::Begin("FPS");
	ImGui::Text("FPS: %.1f", 1.f / ImGui::GetIO().DeltaTime);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

Engine::~Engine()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}