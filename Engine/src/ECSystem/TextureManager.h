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

struct TextureAsset {
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
};

struct TextureHandle
{
	int id = -1;
	bool IsValid() const { return id >= 0; }
};

class TextureManager
{
public:
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

	TextureHandle LoadTexture(const std::string& path, Graphics& gfx);
	std::weak_ptr<TextureAsset> GetTexture(const TextureHandle& handle);

private:
	std::vector<std::shared_ptr<TextureAsset>> m_textures;
	std::unordered_map<std::string, int> m_pathToId;
};

