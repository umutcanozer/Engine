#include "TextureManager.h"
#include "DirectXTK/WICTextureLoader.h"

TextureHandle TextureManager::LoadTexture(const std::string& path, Graphics& gfx)
{
	auto it = m_pathToId.find(path);
	if (it != m_pathToId.end()) {
		return TextureHandle{ it->second };
	}

	std::shared_ptr<TextureAsset> textureAsset = std::make_shared<TextureAsset>();
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	HRESULT hr = DirectX::CreateWICTextureFromFile(
		gfx.GetDevice(),
		gfx.GetContext(),
		std::wstring(path.begin(), path.end()).c_str(),
		resource.GetAddressOf(),
		textureAsset->srv.GetAddressOf()
	);

	if (FAILED(hr)) {
		throw std::runtime_error("Texture couldnt be loaded: " + path);
	}

	resource->QueryInterface(IID_PPV_ARGS(&textureAsset->texture));

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = gfx.GetDevice()->CreateSamplerState(&samplerDesc, &textureAsset->sampler);
	if (FAILED(hr)) {
		throw std::runtime_error("Sampler couldnt be created: " + path);
	}

	int newID = static_cast<int>(m_textures.size());
	m_textures.push_back(textureAsset);
	m_pathToId[path] = newID;
	return TextureHandle{ newID };
}

std::weak_ptr<TextureAsset> TextureManager::GetTexture(const TextureHandle& handle)
{
	if (!handle.IsValid() && handle.id >= m_textures.size()) {
		return std::weak_ptr<TextureAsset>{};
	}
	return m_textures[handle.id];
}
