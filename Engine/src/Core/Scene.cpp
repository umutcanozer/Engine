#include "Scene.h"

Scene::Scene(Graphics& gfx, System& system) : m_graphics(gfx), m_system(system)
{
	m_renderer = std::make_unique<Renderer>(m_graphics, m_registry);
	m_cbufferSystem = std::make_unique<CBufferSystem>(m_graphics, m_registry);
	m_meshSystem = std::make_unique<MeshSystem>(m_graphics, m_registry);
	m_cameraSystem = std::make_unique<CameraSystem>(m_graphics, m_registry);
	m_transformSystem = std::make_unique<TransformSystem>();
	m_cameraController = std::make_unique<CameraController>(m_system, m_registry);
	m_behaviourSystem = std::make_unique<BehaviourSystem>(m_registry);

	m_grid = std::make_unique<Grid>(m_graphics, m_registry);
	m_skybox = std::make_unique<Skybox>(m_graphics, m_registry);

	m_camera = m_registry.create();
	m_gridEntity = m_registry.create();
	m_skyboxEntity = m_registry.create();
}

void Scene::Init()
	{
	auto& meshManager = MeshManager::GetInstance();
	auto& textureManager = TextureManager::GetInstance();

	auto rifleMesh = meshManager.LoadModel("assets/rifle/rifle.obj", m_graphics);
	auto rifleTexture = textureManager.LoadTexture("assets/rifle/m_rifl.bmp", m_graphics);

	entt::entity rifle1 = m_registry.create();
	entt::entity rifle2 = m_registry.create();

	auto& cameraComponent = m_registry.emplace<CameraComponent>(m_camera);
	auto& camTransform = m_registry.emplace<TransformComponent>(m_camera);
	camTransform.transform.position = { 0.0f, 0.0f, -5.0f };
	camTransform.transform.rotation = { 0.f, 0.0f, 0.0f };

	cameraComponent.fov = DirectX::XMConvertToRadians(45.0f);
	cameraComponent.nearPlane = 1.0f;
	cameraComponent.farPlane = 700.0f;
	cameraComponent.aspectRatio = 800.f / 600.f;

	auto& gridComponent = m_registry.emplace<GridComponent>(m_gridEntity);
	//2x2 grid
	gridComponent.vertices = {
	{{-1.0f, 0.0f, -1.0f}}, 
	{{-1.0f, 0.0f,  1.0f}},
	{{ 1.0f, 0.0f,  1.0f}},
	{{ 1.0f, 0.0f, -1.0f}}  
	};

	gridComponent.indices = {
	0, 1, 2,
	0, 2, 3
	};

	auto& gridShader = m_registry.emplace<ShaderComponent>(m_gridEntity);
	gridShader.vertexShaderPath = L"src/Shader/GridVS.hlsl";
	gridShader.pixelShaderPath = L"src/Shader/GridPS.hlsl";
	gridShader.layout = std::vector<D3D11_INPUT_ELEMENT_DESC>{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(GridVertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	auto& gridTransform = m_registry.emplace<TransformComponent>(m_gridEntity);
	gridTransform.transform.position = { 0.f, 0.0f, 0.0f };
	gridTransform.transform.rotation = { 0.0f, 0.0f, 0.0f };
	gridTransform.transform.scale = { 1000.0f, 1.0f, 1000.0f };

	m_registry.emplace<ConstantBufferComponent>(m_gridEntity);

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

	auto& behaviour = m_registry.emplace<BehaviourComponent>(rifle1);
	behaviour.updateFunction = [](entt::entity entity, entt::registry& registry, float deltaTime) {
		auto& transform = registry.get<TransformComponent>(entity);
		float movementSpeed = 150.f;
		float rotationSpeed = 25.f;
		transform.transform.position.x = sinf(static_cast<float>(GetTickCount64() / 1000.0f)) * deltaTime * movementSpeed;
		transform.transform.rotation.y = static_cast<float>(GetTickCount64() / 1000.0f) * deltaTime * rotationSpeed;
		};

	auto& rifleTransform1 = m_registry.emplace<TransformComponent>(rifle1);
	rifleTransform1.transform.position = { -1.0f, 0.0f, 15.0f };
	rifleTransform1.transform.rotation = { 0.0f, 0.0f, 0.0f };
	rifleTransform1.transform.scale = { 0.1f, 0.1f, 0.1f };

	m_registry.emplace<ConstantBufferComponent>(rifle1);

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

	auto& behaviour2 = m_registry.emplace<BehaviourComponent>(rifle2);
	behaviour2.updateFunction = [](entt::entity entity, entt::registry& registry, float deltaTime) {
		auto& transform = registry.get<TransformComponent>(entity);

		float movementSpeed = 350.f;
		float rotationSpeed = 25.f;

		transform.transform.position.y = sinf(static_cast<float>(GetTickCount64() / 1000.0f)) * deltaTime * movementSpeed;
		transform.transform.rotation.y = -static_cast<float>(GetTickCount64() / 1000.0f) * deltaTime * rotationSpeed;
		};

	auto& rifleTransform2 = m_registry.emplace<TransformComponent>(rifle2);
	rifleTransform2.transform.position = { 1.0f, 0.0f, 25.0f };
	rifleTransform2.transform.rotation = { 0.0f, 0.0f, 0.0f };
	rifleTransform2.transform.scale = { 0.1f, 0.1f, 0.1f };

	m_registry.emplace<ConstantBufferComponent>(rifle2);


	std::vector<std::wstring> skyboxFaces = {
		L"assets/skybox/mountain/right.jpg",
		L"assets/skybox/mountain/left.jpg",
		L"assets/skybox/mountain/top.jpg",
		L"assets/skybox/mountain/bottom.jpg",
		L"assets/skybox/mountain/front.jpg",
		L"assets/skybox/mountain/back.jpg"
	};

	m_registry.emplace<SkyboxComponent>(m_skyboxEntity);

	auto& skyboxTransform = m_registry.emplace<TransformComponent>(m_skyboxEntity);
	skyboxTransform.transform.position = { 0.0f, 0.0f, 0.0f };
	skyboxTransform.transform.rotation = { 0.0f, 0.0f, 0.0f };

	m_registry.emplace<ConstantBufferComponent>(m_skyboxEntity);
	
	auto& skyboxShader = m_registry.emplace<ShaderComponent>(m_skyboxEntity);
	skyboxShader.vertexShaderPath = L"src/Shader/Skybox_VS.hlsl";
	skyboxShader.pixelShaderPath = L"src/Shader/Skybox_PS.hlsl";
	skyboxShader.layout = std::vector<D3D11_INPUT_ELEMENT_DESC>{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(SkyboxVertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_cameraSystem->Init();
	m_meshSystem->Init();
	m_cbufferSystem->Init();
	m_grid->Init();
	m_skybox->Init(skyboxFaces);
}

void Scene::Update()
{
	m_renderer->Update();
	m_behaviourSystem->Update(0.016f);
	m_cbufferSystem->Update();
	m_cameraSystem->Update();
	m_cameraController->Update(0.016f);

	auto& gridTransform = m_registry.get<TransformComponent>(m_gridEntity);
	auto& cameraTransform = m_registry.get<TransformComponent>(m_camera);

	gridTransform.transform.position.x = cameraTransform.transform.position.x;
	gridTransform.transform.position.z = cameraTransform.transform.position.z;
}
