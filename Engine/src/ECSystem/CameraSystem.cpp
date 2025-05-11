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

void CameraSystem::Update()
{
	auto view = m_registry.view<CameraComponent, TransformComponent>();
	view.each([&](CameraComponent& camera, TransformComponent& transformComponent) {
		camera.position = XMVectorSet(
			transformComponent.transform.position.x,
			transformComponent.transform.position.y,
			transformComponent.transform.position.z,
			1.0f
		);

		float pitch = XMConvertToRadians(transformComponent.transform.rotation.x);
		float yaw = XMConvertToRadians(transformComponent.transform.rotation.y);
		float roll = XMConvertToRadians(transformComponent.transform.rotation.z);
		//Create a rotation matrix from the pitch, yaw, and roll angles
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

		XMVECTOR forward = XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), rotationMatrix);
		XMVECTOR up = XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), rotationMatrix);
		//camera.target = position + forward vector
		//Assuming forward vector is along the negative Z axis in local space
		//In world space, this would be the position + forward vector
		camera.target = XMVectorAdd(camera.position, forward);
		camera.up = up;

		camera.viewMatrix = XMMatrixLookAtLH(camera.position, camera.target, camera.up);
		camera.projectionMatrix = XMMatrixPerspectiveFovLH(camera.fov, camera.aspectRatio, camera.nearPlane, camera.farPlane);

		CamBuffer data;
		XMStoreFloat4x4(&data.projection, XMMatrixTranspose(camera.projectionMatrix));
		XMStoreFloat4x4(&data.view, XMMatrixTranspose(camera.viewMatrix));
		XMStoreFloat3(&data.position, camera.position);
		data.padding = 0.0f;

		D3D11_MAPPED_SUBRESOURCE mapped;
		if (SUCCEEDED(m_gfx.GetContext()->Map(camera.cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
			memcpy(mapped.pData, &data, sizeof(CamBuffer));
			m_gfx.GetContext()->Unmap(camera.cameraBuffer.Get(), 0);
		}
	});
}

void CameraSystem::CreateCamera(CameraComponent& camera)
{
	D3D11_BUFFER_DESC cbbd{};
	cbbd.Usage = D3D11_USAGE_DYNAMIC;
	cbbd.ByteWidth = sizeof(CamBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbbd.MiscFlags = 0;

	HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&cbbd, NULL, &camera.cameraBuffer);

	if (FAILED(hr)) {
		std::cerr << "Failed to create camera buffer: " << hr << std::endl;
		return;
	}

	//camera.viewMatrix = XMMatrixLookAtLH(camera.position, camera.target, camera.up);
	//camera.projectionMatrix = XMMatrixPerspectiveFovLH(camera.fov, camera.aspectRatio, camera.nearPlane, camera.farPlane);
}
