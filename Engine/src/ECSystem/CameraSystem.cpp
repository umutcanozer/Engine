#include "CameraSystem.h"
using namespace DirectX;

CameraSystem::CameraSystem(Graphics& gfx, entt::registry& registry) 
	: m_gfx(gfx), m_registry(registry)
{
}

void CameraSystem::Init()
{
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView) {
		auto& camera = cameraView.get<CameraComponent>(entity);
		if (!camera.cameraBuffer) {
			CreateCamera(camera);
		}
	}
}

void CameraSystem::Update(float deltaTime)
{
	auto view = m_registry.view<CameraComponent>();
	for (auto entity : view) {
		auto& camera = view.get<CameraComponent>(entity);

		CamBuffer data;

		XMStoreFloat4x4(&data.projection, XMMatrixTranspose(camera.projectionMatrix));
		XMStoreFloat4x4(&data.view, XMMatrixTranspose(camera.viewMatrix));

		m_gfx.GetContext()->UpdateSubresource(camera.cameraBuffer.Get(), 0, NULL, &data, 0, 0);
	}
}

void CameraSystem::CreateCamera(CameraComponent& camera)
{
	D3D11_BUFFER_DESC cbbd{};
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(CamBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&cbbd, NULL, &camera.cameraBuffer);

	if (FAILED(hr)) {
		std::cerr << "Failed to create camera buffer: " << hr << std::endl;
		return;
	}

	camera.viewMatrix = XMMatrixLookAtLH(camera.position, camera.target, camera.up);
	camera.projectionMatrix = XMMatrixPerspectiveFovLH(camera.fov, camera.aspectRatio, camera.nearPlane, camera.farPlane);
}
