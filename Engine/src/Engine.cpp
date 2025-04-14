#include "Engine.h"	
using namespace DirectX;

Engine::Engine()
{
	m_system = std::make_unique<System>();
	m_graphics = std::make_unique<Graphics>(m_system->GetWindow(), m_system->GetWindowWidth(), m_system->GetWindowHeight());
	m_renderSystem = std::make_unique<RenderSystem>(*m_graphics, m_registry);
	m_cbufferSystem = std::make_unique<CBufferSystem>(*m_graphics, m_registry);

	cube = m_registry.create();

	const std::vector<Vertex> vertices =
	{
		// Front (Z+)
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(1,1,1,1) },
		// Back (Z-)
		{ XMFLOAT3(-1.0f,  1.0f, 1.0f),   XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, 1.0f),   XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT4(1,1,1,1) },
		// Left (X-)
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f),  XMFLOAT4(1,1,1,1) },
		// Right (X+)
		{ XMFLOAT3(1.0f,  1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(1.0f, -1.0f,  1.0f),  XMFLOAT4(1,1,1,1) },
		// Top (Y+)
		{ XMFLOAT3(-1.0f, 1.0f,  1.0f),   XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f, 1.0f,  1.0f),   XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT4(1,1,1,1) },
		// Bottom (Y-)
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f, -1.0f,  1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(1,1,1,1) }
	};

	const std::vector<unsigned short> indices =
	{
		0, 1, 2,  0, 2, 3,
		4, 6, 5,  4, 7, 6,
		8, 9, 10,  8, 10, 11,
		12, 14, 13,  12, 15, 14,
		16, 17, 18,  16, 18, 19,
		20, 22, 21,  20, 23, 22
	};
	auto& mesh = m_registry.emplace<MeshComponent>(cube);
	mesh.vertices = vertices;
	mesh.indices = indices;
	mesh.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	auto& shader = m_registry.emplace<ShaderComponent>(cube);
	shader.vertexShaderPath = L"src/Shader/VertexShader.hlsl";
	shader.pixelShaderPath = L"src/Shader/PixelShader.hlsl";
	shader.layout = std::vector<D3D11_INPUT_ELEMENT_DESC>{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto& constantBuffer = m_registry.emplace<ConstantBufferComponent>(cube);
	
	m_renderSystem->Init();
	m_cbufferSystem->Init();
}

void Engine::Frame()
{
	m_graphics->BeginFrame(0.2f, 0.4f, 1.0f, 1.0f);
	
	m_renderSystem->Update();
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