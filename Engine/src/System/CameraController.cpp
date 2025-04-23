#include "CameraController.h"
#include "DirectXMath.h"
using namespace DirectX;
CameraController::CameraController(System& system, entt::registry& registry) : m_system(system),
m_registry(registry)
{
}

void CameraController::Update(float deltaTime)
{
	auto& keyboard = m_system.GetKeyboard();
	auto& mouse = m_system.GetMouse();

	auto view = m_registry.view<TransformComponent, CameraComponent>();
	view.each([&](TransformComponent& transformComponent, CameraComponent& camera) {
		float speed = m_moveSpeed * deltaTime;

		float pitch = XMConvertToRadians(transformComponent.transform.rotation.x);
		float yaw = XMConvertToRadians(transformComponent.transform.rotation.y);
		float roll = XMConvertToRadians(transformComponent.transform.rotation.z);

		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		XMVECTOR forward = XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), rotationMatrix);
		XMVECTOR right = XMVector3TransformNormal(XMVectorSet(1, 0, 0, 0), rotationMatrix);

		XMVECTOR pos = XMLoadFloat3(&transformComponent.transform.position);


		if (keyboard.KeyIsPressed('W'))
			pos = XMVectorAdd(pos, XMVectorScale(forward, speed));
		if (keyboard.KeyIsPressed('S'))
			pos = XMVectorSubtract(pos, XMVectorScale(forward, speed));
		if (keyboard.KeyIsPressed('A'))
			pos = XMVectorSubtract(pos, XMVectorScale(right, speed));
		if (keyboard.KeyIsPressed('D'))
			pos = XMVectorAdd(pos, XMVectorScale(right, speed));

		XMStoreFloat3(&transformComponent.transform.position, pos);

		auto deltaOpt = m_system.GetMouse().ReadRawDelta();
		if (deltaOpt) {
			auto delta = *deltaOpt;
			float dx = static_cast<float>(delta.x);
			float dy = static_cast<float>(delta.y);
			float sensivity = m_rotationSpeed * deltaTime;
			transformComponent.transform.rotation.y += dx * sensivity;
			transformComponent.transform.rotation.x += dy * sensivity;
			transformComponent.transform.rotation.x = std::clamp(transformComponent.transform.rotation.x, -90.f, 90.f);
		}
	});

}
