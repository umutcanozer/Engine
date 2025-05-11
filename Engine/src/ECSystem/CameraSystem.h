#pragma once 
#include "Core/Graphics.h"
#include "TransformSystem.h"
#include "entt.hpp"

struct CameraComponent {
	Microsoft::WRL::ComPtr<ID3D11Buffer> cameraBuffer;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;

	DirectX::XMVECTOR position;
	DirectX::XMVECTOR target;
	DirectX::XMVECTOR up;

	float fov;
	float nearPlane;
	float farPlane;
	float aspectRatio;

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
};

struct CamBuffer {
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT3 position;
	float padding;
};


class CameraSystem {
public:
	CameraSystem(Graphics& gfx, entt::registry& registry);
	CameraSystem(const CameraSystem&) = delete;
	CameraSystem& operator=(const CameraSystem&) = delete;
	~CameraSystem() = default;
	void Init();
	void Update();

private:
	void CreateCamera(CameraComponent& camera);

private:
	Graphics& m_gfx;
	entt::registry& m_registry;
};