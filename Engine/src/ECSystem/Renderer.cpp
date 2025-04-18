#include "Renderer.h"

Renderer::Renderer(Graphics& gfx, entt::registry& registry) : m_gfx(gfx), m_registry(registry)
{
	m_Stride = sizeof(Vertex);
	m_Offset = 0;
}

void Renderer::Update()
{
	auto view = m_registry.group<MeshComponent, ShaderComponent, TextureComponent, ConstantBufferComponent>();
	for (auto entity : view) {
		auto& mesh = view.get<MeshComponent>(entity);


		auto meshPtr = mesh.meshAsset.lock();
		if (!meshPtr) continue;
		auto& texture = view.get<TextureComponent>(entity);
		auto texturePtr = texture.textureAsset.lock();
		if (!texturePtr) continue;

		auto& shader = view.get<ShaderComponent>(entity);
		auto& constantBuffer = view.get<ConstantBufferComponent>(entity);

		auto context = m_gfx.GetContext();
		ID3D11Buffer* vertexBuffer = meshPtr->vertexBuffer.Get();

		context->IASetInputLayout(shader.inputLayout.Get());
		context->IASetPrimitiveTopology(mesh.topology);
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &m_Stride, &m_Offset);
		context->IASetIndexBuffer(meshPtr->indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		context->VSSetShader(shader.vertexShader.Get(), nullptr, 0);
		context->PSSetShader(shader.pixelShader.Get(), nullptr, 0);

		if (auto tex = texture.textureAsset.lock()) {
			context->PSSetShaderResources(0, 1, tex->srv.GetAddressOf());     // t0
			context->PSSetSamplers(0, 1, tex->sampler.GetAddressOf());       // s0
		}

		context->VSSetConstantBuffers(0, 1, constantBuffer.matrixBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, constantBuffer.matrixBuffer.GetAddressOf());

		context->DrawIndexed((UINT)meshPtr->indices.size(), 0, 0);
	}
}

