#include "Engine.h"	

using namespace DirectX;

Engine::Engine()
{
	m_system = std::make_unique<System>();
	m_graphics = std::make_unique<Graphics>(m_system->GetWindow(), m_system->GetWindowWidth(), m_system->GetWindowHeight());

	m_renderer = std::make_unique<Renderer>(*m_graphics, m_registry);
	m_cbufferSystem = std::make_unique<CBufferSystem>(*m_graphics, m_registry);
	m_meshSystem = std::make_unique<MeshSystem>(*m_graphics, m_registry);

	auto& meshManager = MeshManager::GetInstance();
	auto rifleMesh = meshManager.LoadModel("assets/rifle/rifle.obj", *m_graphics);
	auto rabbitMesh = meshManager.LoadModel("assets/wally/wally_uv.obj", *m_graphics);

	entt::entity cube = m_registry.create();
	entt::entity cube2 = m_registry.create();

	auto& mesh = m_registry.emplace<MeshComponent>(cube);
	mesh.meshAsset = meshManager.GetMesh(rifleMesh);
	mesh.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	auto& shader = m_registry.emplace<ShaderComponent>(cube);
	shader.vertexShaderPath = L"src/Shader/VertexShader.hlsl";
	shader.pixelShaderPath = L"src/Shader/PixelShader.hlsl";
	shader.layout = std::vector<D3D11_INPUT_ELEMENT_DESC>{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto& constantBuffer = m_registry.emplace<ConstantBufferComponent>(cube);
	constantBuffer.offsetZ = 10.0f;
	constantBuffer.offsetX = -1.0f;
	constantBuffer.offsetY = -3.0f;
	
	auto& mesh2 = m_registry.emplace<MeshComponent>(cube2);
	mesh2.meshAsset = meshManager.GetMesh(rifleMesh);
	mesh2.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	auto& shader2 = m_registry.emplace<ShaderComponent>(cube2);
	shader2.vertexShaderPath = L"src/Shader/VertexShader.hlsl";
	shader2.pixelShaderPath = L"src/Shader/PixelShader.hlsl";
	shader2.layout = std::vector<D3D11_INPUT_ELEMENT_DESC>{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto& constantBuffer2 = m_registry.emplace<ConstantBufferComponent>(cube2);
	constantBuffer2.offsetZ = 5.0f;
	constantBuffer2.offsetX = 1.0f;
	constantBuffer2.offsetY = 3.0f;
	
	m_meshSystem->Init();
	m_cbufferSystem->Init();
}

void Engine::Frame()
{
	m_graphics->BeginFrame(0.2f, 0.4f, 1.0f, 1.0f);
	
	m_renderer->Update();
	m_cbufferSystem->Update();

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