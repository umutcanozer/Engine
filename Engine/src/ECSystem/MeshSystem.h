#pragma once
#include "Core/Graphics.h"
#include "entt.hpp"
#include "MeshManager.h"
#include "TextureManager.h"

struct MeshComponent {
	std::weak_ptr<MeshAsset> meshAsset;
	D3D11_PRIMITIVE_TOPOLOGY topology;
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

struct TextureComponent {
	std::weak_ptr<TextureAsset> textureAsset;
};


class MeshSystem {

public:
	MeshSystem(Graphics& gfx, entt::registry& registry);
	MeshSystem(const MeshSystem&) = delete;
	MeshSystem& operator=(const MeshSystem&) = delete;
	~MeshSystem() = default;
	void Init();

private:
	void CreateShaders(ShaderComponent& shader);

	Graphics& m_gfx;
	entt::registry& m_registry;
};