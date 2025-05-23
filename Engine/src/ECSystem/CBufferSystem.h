#pragma once
#include "Core/Graphics.h"
#include "TransformSystem.h"
#include "entt.hpp"
#include <vector>


struct MatrixBuffer {
	DirectX::XMFLOAT4X4 world;
};

struct ConstantBufferComponent {
	Microsoft::WRL::ComPtr<ID3D11Buffer> matrixBuffer;
	MatrixBuffer matrixData;

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