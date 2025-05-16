#pragma once
#include <vector>
#include "entt.hpp"
#include "Core/Graphics.h"


struct SkyboxVertex {
    DirectX::XMFLOAT3 position;
};

struct SkyboxComponent {
    std::vector<SkyboxVertex> vertices;
    std::vector<unsigned short> indices;
    UINT indexCount = 0;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeMapSRV;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
};

class Skybox
{
public:
	Skybox(Graphics& gfx, entt::registry& registry);
	Skybox(const Skybox&) = delete;
	Skybox& operator=(const Skybox&) = delete;
	~Skybox() = default;

	void Init(const std::vector<std::wstring>& faces);

private:
	void CreateSkyboxBuffers(SkyboxComponent& comp, const std::vector<std::wstring>& faces);

private:
	Graphics& m_gfx;
    entt::registry& m_registry;
};

