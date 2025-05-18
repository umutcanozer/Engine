#include "Renderer.h"

Renderer::Renderer(Graphics& gfx, entt::registry& registry) : m_gfx(gfx), m_registry(registry)
{
	m_Offset = 0;

	InitDepthStates();
	InitBlendState();
	InitRasterizers();
}

void Renderer::InitDepthStates()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	HRESULT hr = m_gfx.GetDevice()->CreateDepthStencilState(&dsDesc, &m_defaultDepthState);
	if (FAILED(hr)) {
		std::cerr << "[Renderer] Failed to create default depth stencil state!\n";
	}

	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_gfx.GetDevice()->CreateDepthStencilState(&dsDesc, &m_maskZeroState);
	if (FAILED(hr)) {
		std::cerr << "[Renderer] Failed to create grid depth stencil state!\n";
	}
}

void Renderer::InitBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = m_gfx.GetDevice()->CreateBlendState(&blendDesc, &m_gridBlendState);
	if (FAILED(hr)) {
		std::cerr << "Failed to create grid blend state!\n";
	}	
}

void Renderer::InitRasterizers()
{
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthClipEnable = FALSE;

	HRESULT hr = m_gfx.GetDevice()->CreateRasterizerState(&rsDesc, &m_skyboxRasterState);
	if (FAILED(hr)) {
		std::cerr << "Failed to create skybox rasterizer state!\n";
	}

	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.DepthClipEnable = TRUE;

	hr = m_gfx.GetDevice()->CreateRasterizerState(&rsDesc, &m_gridRasterState);
	if (FAILED(hr)) {
		std::cerr << "Failed to create grid rasterizer state!\n";
	}
}

void Renderer::Update()
{
	auto context = m_gfx.GetContext();

	auto lightView = m_registry.view<LightComponent>();
	for (auto entity : lightView) {
		auto& light = lightView.get<LightComponent>(entity);
		D3D11_MAPPED_SUBRESOURCE mapped{};
		if (SUCCEEDED(context->Map(light.lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) {
			memcpy(mapped.pData, &light.lightData, sizeof(LightBuffer));
			context->Unmap(light.lightBuffer.Get(), 0);
		}
		context->PSSetConstantBuffers(2, 1, light.lightBuffer.GetAddressOf());
	}

	
	auto skyboxView = m_registry.view<SkyboxComponent, ShaderComponent, ConstantBufferComponent>();
	for (auto [entity, skybox, shader, cbuffer] : skyboxView.each()) {
		context->RSSetState(m_skyboxRasterState.Get());
		context->OMSetDepthStencilState(m_maskZeroState.Get(), 1u); 
		context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

		m_Stride = sizeof(SkyboxVertex);
		context->IASetInputLayout(shader.inputLayout.Get());

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetVertexBuffers(0, 1, skybox.vertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);
		context->IASetIndexBuffer(skybox.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		context->VSSetShader(shader.vertexShader.Get(), nullptr, 0);
		context->PSSetShader(shader.pixelShader.Get(), nullptr, 0);

		context->VSSetConstantBuffers(0, 1, cbuffer.matrixBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, cbuffer.matrixBuffer.GetAddressOf());

		context->PSSetShaderResources(0, 1, skybox.cubeMapSRV.GetAddressOf());
		context->PSSetSamplers(0, 1, skybox.samplerState.GetAddressOf());

		context->DrawIndexed(skybox.indexCount, 0, 0);
		context->RSSetState(nullptr);
	}

	auto view = m_registry.view<ShaderComponent, ConstantBufferComponent>();
	for (auto [entity, shader, cbuffer] : view.each()) {
		if (m_registry.any_of<SkyboxComponent>(entity)) continue;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vb = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> ib = nullptr;
		UINT indexCount = 0;
		D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		if (m_registry.any_of<MeshComponent>(entity)) {
			context->RSSetState(nullptr);
			context->OMSetDepthStencilState(m_defaultDepthState.Get(), 1u);
			context->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

			m_Stride = sizeof(Vertex);
			auto& mesh = m_registry.get<MeshComponent>(entity);
			auto meshPtr = mesh.meshAsset.lock();
			if (!meshPtr) continue;
			vb = meshPtr->vertexBuffer.Get();
			ib = meshPtr->indexBuffer.Get();
			indexCount = (UINT)meshPtr->indices.size();
			topology = mesh.topology;

			auto textureView = m_registry.try_get<TextureComponent>(entity);
			if (textureView) {
				auto texturePtr = textureView->textureAsset.lock();
				if (texturePtr) {
					context->PSSetShaderResources(0, 1, texturePtr->srv.GetAddressOf());    
					context->PSSetSamplers(0, 1, texturePtr->sampler.GetAddressOf());       
				}
			}
		}
		else if (m_registry.any_of<GridComponent>(entity)) {
			context->RSSetState(m_gridRasterState.Get());
			context->OMSetDepthStencilState(m_maskZeroState.Get(), 1u);
			context->OMSetBlendState(m_gridBlendState.Get(), blendFactor, 0xFFFFFFFF);

			m_Stride = sizeof(GridVertex);
			auto& grid = m_registry.get<GridComponent>(entity);
			vb = grid.vertexBuffer.Get();
			ib = grid.indexBuffer.Get();
			indexCount = (UINT)grid.indices.size();
		}
		else {
			continue;
		}

		context->IASetInputLayout(shader.inputLayout.Get());
		context->IASetPrimitiveTopology(topology);
		context->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &m_Stride, &m_Offset);
		context->IASetIndexBuffer(ib.Get(), DXGI_FORMAT_R16_UINT, 0);
		context->VSSetShader(shader.vertexShader.Get(), nullptr, 0);
		context->PSSetShader(shader.pixelShader.Get(), nullptr, 0);

		context->VSSetConstantBuffers(0, 1, cbuffer.matrixBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, cbuffer.matrixBuffer.GetAddressOf());

		context->DrawIndexed(indexCount, 0, 0);

	}

	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView) {
		auto& camera = cameraView.get<CameraComponent>(entity);
		auto context = m_gfx.GetContext();
		context->VSSetConstantBuffers(1, 1, camera.cameraBuffer.GetAddressOf());
		context->PSSetConstantBuffers(1, 1, camera.cameraBuffer.GetAddressOf());
	}
}

