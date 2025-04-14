#include "MeshManager.h"
using namespace DirectX;
std::shared_ptr<MeshAsset> MeshManager::LoadMesh(const std::string& path, Graphics& gfx)
{
    if (m_loadedMeshes.contains(path)) {
        return m_loadedMeshes[path];
    }

	std::shared_ptr<MeshAsset> mesh = std::make_shared<MeshAsset>();
	mesh->vertices =
	{
		// Front (Z+)
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(1,1,1,1) },
		// Back (Z-)
		{ XMFLOAT3(-1.0f,  1.0f, 1.0f),   XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, 1.0f),   XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),   XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT4(1,1,1,1) },
		// Left (X-)
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f),  XMFLOAT4(1,1,1,1) },
		// Right (X+)
		{ XMFLOAT3(1.0f,  1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f,  1.0f, -1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(1.0f, -1.0f,  1.0f),  XMFLOAT4(1,1,1,1) },
		// Top (Y+)
		{ XMFLOAT3(-1.0f, 1.0f,  1.0f),   XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f, 1.0f,  1.0f),   XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),   XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT4(1,1,1,1) },
		// Bottom (Y-)
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f),  XMFLOAT4(1,0,0,1) },
		{ XMFLOAT3(1.0f, -1.0f,  1.0f),  XMFLOAT4(0,1,0,1) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f),  XMFLOAT4(0,0,1,1) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT4(1,1,1,1) }
	};

	mesh->indices =
	{
		0, 1, 2,  0, 2, 3,
		4, 6, 5,  4, 7, 6,
		8, 9, 10,  8, 10, 11,
		12, 14, 13,  12, 15, 14,
		16, 17, 18,  16, 18, 19,
		20, 22, 21,  20, 23, 22
	};

	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.ByteWidth = (UINT)(sizeof(Vertex) * mesh->vertices.size());
	vbDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = mesh->vertices.data();

	gfx.GetDevice()->CreateBuffer(&vbDesc, &vbData, &mesh->vertexBuffer);

	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.ByteWidth = (UINT)(sizeof(unsigned short) * mesh->indices.size());
	ibDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = mesh->indices.data();

	gfx.GetDevice()->CreateBuffer(&ibDesc, &ibData, &mesh->indexBuffer);
	m_loadedMeshes[path] = mesh;

    return mesh;
}
