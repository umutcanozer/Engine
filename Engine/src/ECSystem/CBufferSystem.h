#pragma once
#include "Core/Graphics.h"
#include "entt.hpp"
#include <vector>

struct MatrixBuffer {
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};

struct ConstantBufferComponent {
	Microsoft::WRL::ComPtr<ID3D11Buffer> matrixBuffer;
	MatrixBuffer matrixData;

	float offsetZ = 0.0f;
	float offsetX = 0.0f;

	ConstantBufferComponent() = default;
	ConstantBufferComponent(const ConstantBufferComponent&) = default;
};

class CBufferSystem
{
public:
	CBufferSystem(Graphics& gfx, entt::registry& registry);
	CBufferSystem(const CBufferSystem&) = delete;
	CBufferSystem& operator=(const CBufferSystem&) = delete;
	~CBufferSystem() = default;

	void Init();
	void Update();

private:
	void CreateConstants(ConstantBufferComponent& constantBuffer);
private:
	Graphics& m_gfx;
	entt::registry& m_registry;
};