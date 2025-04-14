#include "RenderSystem.h"

RenderSystem::RenderSystem(Graphics& gfx, entt::registry& registry) : m_gfx(gfx), m_registry(registry)
{
	m_Stride = sizeof(Vertex);
	m_Offset = 0;
}

void RenderSystem::Init()
{
	auto view = m_registry.view<MeshComponent>();
	for (auto entity : view) {
		auto& mesh = view.get<MeshComponent>(entity);
		if (!mesh.vertexBuffer || !mesh.indexBuffer) {
			CreateBuffers(mesh);
		}
	}

	auto shaderView = m_registry.view<ShaderComponent>();
	for (auto entity : shaderView) {
		auto& shader = shaderView.get<ShaderComponent>(entity);
		if (!shader.vertexShader || !shader.pixelShader || !shader.inputLayout) {
			CreateShaders(shader);
		}
	}
}

void RenderSystem::Update()
{
	auto view = m_registry.group<MeshComponent, ShaderComponent>();
	for (auto entity : view) {
		auto& mesh = view.get<MeshComponent>(entity);
		auto& shader = view.get<ShaderComponent>(entity);

		auto context = m_gfx.GetContext();

		context->IASetInputLayout(shader.inputLayout.Get());
		context->IASetPrimitiveTopology(mesh.topology);
		context->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);
		context->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		context->VSSetShader(shader.vertexShader.Get(), nullptr, 0);
		context->PSSetShader(shader.pixelShader.Get(), nullptr, 0);

		context->DrawIndexed((UINT)mesh.indices.size(), 0, 0);
	}
}

void RenderSystem::CreateBuffers(MeshComponent& mesh)
{
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.ByteWidth = (UINT)(sizeof(Vertex) * mesh.vertices.size());
	vbDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = mesh.vertices.data();

	m_gfx.GetDevice()->CreateBuffer(&vbDesc, &vbData, &mesh.vertexBuffer);

	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.ByteWidth = (UINT)(sizeof(unsigned short) * mesh.indices.size());
	ibDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = mesh.indices.data();

	m_gfx.GetDevice()->CreateBuffer(&ibDesc, &ibData, &mesh.indexBuffer);
}

void RenderSystem::CreateShaders(ShaderComponent& shader)
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

	// Create the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_gfx.GetDevice()->CreateInputLayout(shader.layout.data(), (UINT)shader.layout.size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &shader.inputLayout);
}