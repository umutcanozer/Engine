#pragma once
#include "Core/Graphics.h"
#include "entt.hpp"
#include "MeshSystem.h"
#include "CBufferSystem.h"
#include "CameraSystem.h"
#include "Core/Grid.h"
#include "Core/SkyBox.h"


class Renderer {
public:
    Renderer(Graphics& gfx, entt::registry& registry);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	~Renderer() = default;

    void InitDepthStates();
    void InitBlendState();
	void InitRasterizers();
    void Update();

private:
    Graphics& m_gfx;
    entt::registry& m_registry;

    UINT m_Stride;
    UINT m_Offset;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_defaultDepthState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_maskZeroState;

    Microsoft::WRL::ComPtr<ID3D11BlendState> m_gridBlendState;
    const float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_skyboxRasterState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_gridRasterState;
};
