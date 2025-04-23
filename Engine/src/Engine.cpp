#include "Engine.h"	

using namespace DirectX;

Engine::Engine()
{
	m_system = std::make_unique<System>();
	m_graphics = std::make_unique<Graphics>(m_system->GetWindow(), m_system->GetWindowWidth(), m_system->GetWindowHeight());
	InitImGui();

	m_renderer = std::make_unique<Renderer>(*m_graphics, m_registry);
	m_cbufferSystem = std::make_unique<CBufferSystem>(*m_graphics, m_registry);
	m_meshSystem = std::make_unique<MeshSystem>(*m_graphics, m_registry);
	m_cameraSystem = std::make_unique<CameraSystem>(*m_graphics, m_registry);
	m_transformSystem = std::make_unique<TransformSystem>();
	m_cameraController = std::make_unique<CameraController>(*m_system, m_registry);

	auto& meshManager = MeshManager::GetInstance();
	auto& textureManager = TextureManager::GetInstance();

	auto rifleMesh = meshManager.LoadModel("assets/rifle/rifle.obj", *m_graphics);
	auto rifleTexture = textureManager.LoadTexture("assets/rifle/m_rifl.bmp", *m_graphics);


	entt::entity camera = m_registry.create();
	entt::entity rifle1 = m_registry.create();
	entt::entity rifle2 = m_registry.create();

	auto& cameraComponent = m_registry.emplace<CameraComponent>(camera);
	auto& camTransform = m_registry.emplace<TransformComponent>(camera);
	camTransform.transform.position = { 0.0f, 0.0f, -5.0f };
	camTransform.transform.rotation = { 0.0f, 0.0f, 0.0f };

	//cameraComponent.position = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
	//cameraComponent.target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//cameraComponent.up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	cameraComponent.fov = XMConvertToRadians(45.0f);
	cameraComponent.nearPlane = 1.0f;
	cameraComponent.farPlane = 100.0f;
	cameraComponent.aspectRatio = 800.f / 600.f;

	auto& mesh = m_registry.emplace<MeshComponent>(rifle1);
	mesh.meshAsset = meshManager.GetMesh(rifleMesh);
	mesh.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	auto& shader = m_registry.emplace<ShaderComponent>(rifle1);
	shader.vertexShaderPath = L"src/Shader/VertexShader.hlsl";
	shader.pixelShaderPath = L"src/Shader/PixelShader.hlsl";
	shader.layout = std::vector<D3D11_INPUT_ELEMENT_DESC>{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto& texture = m_registry.emplace<TextureComponent>(rifle1);
	texture.textureAsset = textureManager.GetTexture(rifleTexture);

	auto& rifleTransform1 = m_registry.emplace<TransformComponent>(rifle1);
	m_registry.emplace<Movable>(rifle1);
	rifleTransform1.transform.position = { -1.0f, 0.0f, 15.0f };
	rifleTransform1.transform.rotation = { 0.0f, 0.0f, 0.0f };
	rifleTransform1.transform.scale = { 0.1f, 0.1f, 0.1f };

	auto& constantBuffer = m_registry.emplace<ConstantBufferComponent>(rifle1);

	auto& mesh2 = m_registry.emplace<MeshComponent>(rifle2);
	mesh2.meshAsset = meshManager.GetMesh(rifleMesh);
	mesh2.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	auto& shader2 = m_registry.emplace<ShaderComponent>(rifle2);
	shader2.vertexShaderPath = L"src/Shader/VertexShader.hlsl";
	shader2.pixelShaderPath = L"src/Shader/PixelShader.hlsl";
	shader2.layout = std::vector<D3D11_INPUT_ELEMENT_DESC>{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(Vertex, texcoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto& texture2 = m_registry.emplace<TextureComponent>(rifle2);
	texture2.textureAsset = textureManager.GetTexture(rifleTexture);

	auto& rifleTransform2 = m_registry.emplace<TransformComponent>(rifle2);
	m_registry.emplace<Movable>(rifle2);
	rifleTransform2.transform.position = { 1.0f, 0.0f, 25.0f };
	rifleTransform2.transform.rotation = { 0.0f, 0.0f, 0.0f };
	rifleTransform2.transform.scale = { 0.1f, 0.1f, 0.1f };

	auto& constantBuffer2 = m_registry.emplace<ConstantBufferComponent>(rifle2);
	
	m_cameraSystem->Init();
	m_meshSystem->Init();
	m_cbufferSystem->Init();
}

void Engine::Frame()
{
	Transform testTransform = {
		.position = { 0.0f, sinf(static_cast<float>(GetTickCount64() / 1000.0f)), 0.0f },
		.rotation = { 0.0f, static_cast<float>(GetTickCount64() / 1000.0f), 0.0f },
		.scale = { 0.1f, 0.1f, 0.1f }
	};
	m_graphics->BeginFrame(0.2f, 0.4f, 1.0f, 1.0f);
	m_renderer->Update();
	m_transformSystem->Update(m_registry, testTransform);
	m_cbufferSystem->Update();
	m_cameraSystem->Update(); 
	m_cameraController->Update(0.016f); 
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