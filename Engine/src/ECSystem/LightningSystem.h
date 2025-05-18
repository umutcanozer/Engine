#pragma once
#include "Core/Graphics.h"
#include "entt.hpp"

struct LightBuffer
{
	DirectX::XMFLOAT3 position;
	float range;
	DirectX::XMFLOAT3 attenuation;
	float intensity;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
};

struct LightComponent {
	Microsoft::WRL::ComPtr<ID3D11Buffer> lightBuffer;
	LightBuffer lightData;
};

class LightningSystem
{
public:
	LightningSystem(Graphics& gfx, entt::registry& registry);
	LightningSystem(const LightningSystem&) = delete;
	LightningSystem& operator=(const LightningSystem&) = delete;
	~LightningSystem() = default;

	void Init();
	void Update();

private:
	void CreateLight(LightComponent& light);

private:
	Graphics& m_gfx;
	entt::registry& m_registry;
};

