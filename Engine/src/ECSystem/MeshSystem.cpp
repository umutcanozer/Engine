#include "MeshSystem.h"

MeshSystem::MeshSystem(Graphics& gfx, entt::registry& registry) : m_gfx(gfx), m_registry(registry)
{
}

void MeshSystem::Init()
{
	auto shaderView = m_registry.view<ShaderComponent>();
	for (auto entity : shaderView) {
		auto& shader = shaderView.get<ShaderComponent>(entity);
		if (!shader.vertexShader || !shader.pixelShader || !shader.inputLayout) {
			CreateShaders(shader);
		}
	}
}

void MeshSystem::CreateShaders(ShaderComponent& shader)
{
	if (shader.layout.empty()) {
		std::cerr << "[ERROR] Shader layout is empty! InputLayout cannot be created!" << std::endl;
		return;
	}
	// Create the vertex shader
	Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob;
	HRESULT hr = D3DCompileFromFile(shader.vertexShaderPath.c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, &pVSBlob, nullptr);
	if (FAILED(hr)) {
		std::cerr << "D3DCompileFromFile failed with error: " << hr << std::endl;
		return;
	}
	m_gfx.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &shader.vertexShader);

	// Create the pixel shader
	Microsoft::WRL::ComPtr<ID3DBlob> pPSBlob;
	hr = D3DCompileFromFile(shader.pixelShaderPath.c_str(), nullptr, nullptr, "main", "ps_5_0", 0, 0, &pPSBlob, nullptr);
	if (FAILED(hr)) {
		std::cerr << "D3DCompileFromFile failed with error: " << hr << std::endl;
		return;
	}
	m_gfx.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &shader.pixelShader);

	m_gfx.GetDevice()->CreateInputLayout(shader.layout.data(), (UINT)shader.layout.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &shader.inputLayout);
}

