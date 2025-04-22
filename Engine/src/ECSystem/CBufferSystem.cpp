#include "CBufferSystem.h"
using namespace DirectX;
CBufferSystem::CBufferSystem(Graphics& gfx, entt::registry& registry) : m_gfx(gfx), m_registry(registry)
{
}

void CBufferSystem::Init()
{
	auto constantBufferView = m_registry.view<ConstantBufferComponent>();
	for (auto entity : constantBufferView) {
		auto& constantBuffer = constantBufferView.get<ConstantBufferComponent>(entity);
		if (!constantBuffer.matrixBuffer) {
			CreateConstants(constantBuffer);
		}
	}
}

void CBufferSystem::Update()
{
	auto view = m_registry.view<TransformComponent, ConstantBufferComponent>();
	view.each([&](auto entity, TransformComponent tComponent, ConstantBufferComponent constant) {
		XMMATRIX worldMat = XMMatrixScaling(tComponent.transform.scale.x, tComponent.transform.scale.y, tComponent.transform.scale.z) *
			XMMatrixRotationRollPitchYaw(tComponent.transform.rotation.x, tComponent.transform.rotation.y, tComponent.transform.rotation.z) *
			XMMatrixTranslation(tComponent.transform.position.x, tComponent.transform.position.y, tComponent.transform.position.z);

		auto context = m_gfx.GetContext();
		D3D11_MAPPED_SUBRESOURCE mapped;
		HRESULT hr = context->Map(constant.matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		if (FAILED(hr))
		{
			std::cerr << "Map failed with error: " << hr << std::endl;
			return;
		}
		MatrixBuffer* cbDataPtr;
		cbDataPtr = (MatrixBuffer*)mapped.pData;
		XMStoreFloat4x4(&cbDataPtr->world, XMMatrixTranspose(worldMat));
		m_gfx.GetContext()->Unmap(constant.matrixBuffer.Get(), 0);
	});
}

void CBufferSystem::CreateConstants(ConstantBufferComponent& constantBuffer)
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.ByteWidth = sizeof(MatrixBuffer);
	cbDesc.StructureByteStride = 0;
	m_gfx.GetDevice()->CreateBuffer(&cbDesc, nullptr, &constantBuffer.matrixBuffer);
}

