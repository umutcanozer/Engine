#include "LightningSystem.h"

using namespace DirectX;
LightningSystem::LightningSystem(Graphics& gfx, entt::registry& registry)
	: m_gfx(gfx), m_registry(registry)
{
}

void LightningSystem::Init()
{
	auto lightView = m_registry.view<LightComponent>();
	for (auto entity : lightView)
	{
		auto& light = lightView.get<LightComponent>(entity);
		CreateLight(light);
	}
}


void LightningSystem::CreateLight(LightComponent& light)
{
	D3D11_BUFFER_DESC bd{};
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.ByteWidth = sizeof(LightBuffer);
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = &light.lightData;

	HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&bd, &sd, &light.lightBuffer);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create light constant buffer!");
	}
}


void LightningSystem::Update()
{

}