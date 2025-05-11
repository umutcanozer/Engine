#include "Grid.h"

Grid::Grid(Graphics& gfx, entt::registry& registry) : m_gfx(gfx), m_registry(registry)
{

}

void Grid::Init()
{
	auto view = m_registry.view<GridComponent>();
	for (auto entity : view) {
		auto& gridComponent = view.get<GridComponent>(entity);
		CreateGridBuffers(gridComponent);
	}
}

void Grid::CreateGridBuffers(GridComponent& gridComponent)
{
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.ByteWidth = (UINT)(sizeof(GridVertex) * gridComponent.vertices.size());
	vbDesc.StructureByteStride = sizeof(GridVertex);
	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = gridComponent.vertices.data();
	HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&vbDesc, &vbData, &gridComponent.vertexBuffer);
	if (FAILED(hr)) {
		std::cerr << "Failed to create vertex buffer!" << std::endl;
	}

	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.ByteWidth = (UINT)(sizeof(unsigned short) * gridComponent.indices.size());
	ibDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = gridComponent.indices.data();

	hr = m_gfx.GetDevice()->CreateBuffer(&ibDesc, &ibData, &gridComponent.indexBuffer);
	if (FAILED(hr)) {
		std::cerr << "Failed to create index buffer!" << std::endl;
	}
}
