#pragma once
#include "Core/Graphics.h"
#include "entt.hpp"
#include <vector>

struct Vertex {
    DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

struct MeshComponent {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

    D3D11_PRIMITIVE_TOPOLOGY topology;
	MeshComponent() = default;
	MeshComponent(const MeshComponent&) = default;
};

struct ShaderComponent {
	std::wstring vertexShaderPath;
	std::wstring pixelShaderPath;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout;

    ShaderComponent() = default;
	ShaderComponent(const ShaderComponent&) = default;
};

class RenderSystem {
public:
    RenderSystem(Graphics& gfx, entt::registry& registry);
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	~RenderSystem() = default;

    void Init();
    void Update();

private:
    void CreateBuffers(MeshComponent& mesh);
	void CreateShaders(ShaderComponent& shader);

private:

    Graphics& m_gfx;
    entt::registry& m_registry;
    UINT m_Stride;
    UINT m_Offset;
};
