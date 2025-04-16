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
	XMMATRIX viewMat = XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
	);
	XMMATRIX projMat = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	auto view = m_registry.view<ConstantBufferComponent>();
	for (auto entity : view) {
		auto& constantBuffer = view.get<ConstantBufferComponent>(entity);

		XMMATRIX worldMat = XMMatrixScaling(0.1f, 0.1f, 0.1f) *
			XMMatrixRotationY(static_cast<float>(GetTickCount64() / 1000.0f)) *
			XMMatrixTranslation(constantBuffer.offsetX, constantBuffer.offsetY, constantBuffer.offsetZ);

		auto context = m_gfx.GetContext();
		D3D11_MAPPED_SUBRESOURCE mapped;
		HRESULT hr = context->Map(constantBuffer.matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		if (FAILED(hr))
		{
			std::cerr << "Map failed with error: " << hr << std::endl;
			return;
		}
		MatrixBuffer* cbDataPtr;
		cbDataPtr = (MatrixBuffer*)mapped.pData;
		XMStoreFloat4x4(&cbDataPtr->world, XMMatrixTranspose(worldMat));
		XMStoreFloat4x4(&cbDataPtr->view, XMMatrixTranspose(viewMat));
		XMStoreFloat4x4(&cbDataPtr->proj, XMMatrixTranspose(projMat));
		m_gfx.GetContext()->Unmap(constantBuffer.matrixBuffer.Get(), 0);
	}
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

