#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <fstream>
#include "Core/Graphics.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct Vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
	DirectX::XMFLOAT4 color;
};

struct MeshHandle
{
	int id = -1;
	bool IsValid() const { return id >= 0; }
};

struct MeshAsset
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};


class MeshManager
{
public:
	static MeshManager& GetInstance()
	{
		static MeshManager instance;
		return instance;
	}

	MeshHandle LoadModel(const std::string& path, Graphics& gfx);
	std::weak_ptr<MeshAsset> GetMesh(const MeshHandle& handle);

private:
	std::vector<std::shared_ptr<MeshAsset>> m_meshes;
	std::unordered_map<std::string, int> m_pathToId;
};

