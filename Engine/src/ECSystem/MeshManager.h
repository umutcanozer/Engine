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
	DirectX::XMFLOAT4 color;
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

	std::shared_ptr<MeshAsset> LoadModel(const std::string& path, Graphics& gfx);

private:
	std::unordered_map<std::string, std::shared_ptr<MeshAsset>> m_loadedMeshes;
};

