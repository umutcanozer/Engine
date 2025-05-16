#include "SkyBox.h"
#include <DirectXTK/WICTextureLoader.h>

using namespace DirectX;

Skybox::Skybox(Graphics& gfx, entt::registry& registry) : m_gfx(gfx), m_registry(registry) 
{
}

void Skybox::Init(const std::vector<std::wstring>& faces)
{
	auto view = m_registry.view<SkyboxComponent>();
    for (auto entity : view) {
        auto& skyboxComponent = view.get<SkyboxComponent>(entity);
        CreateSkyboxBuffers(skyboxComponent, faces);
    }
}

void Skybox::CreateSkyboxBuffers(SkyboxComponent& comp, const std::vector<std::wstring>& faces)
{
    // 1) Create cube mesh
    comp.vertices = {
        {{-1,  1, -1}}, {{ 1,  1, -1}}, {{ 1, -1, -1}}, {{-1, -1, -1}},
        {{-1,  1,  1}}, {{ 1,  1,  1}}, {{ 1, -1,  1}}, {{-1, -1,  1}}
    };
    comp.indices = {
        0,1,2, 0,2,3,
        4,6,5, 4,7,6,
        4,5,1, 4,1,0,
        3,2,6, 3,6,7,
        4,0,3, 4,3,7,
        1,5,6, 1,6,2
    };
    comp.indexCount = (UINT)comp.indices.size();


    //2) Create buffers
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.ByteWidth = sizeof(SkyboxVertex) * comp.vertices.size();

    D3D11_SUBRESOURCE_DATA sd{};
    sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;
	sd.pSysMem = comp.vertices.data();
 

    HRESULT hr = m_gfx.GetDevice()->CreateBuffer(&bd, &sd, &comp.vertexBuffer);
    if (FAILED(hr)) {
        std::stringstream ss;
        ss << "DX Error! HRESULT = 0x" << std::hex << hr;
        throw std::runtime_error(ss.str());
    }


    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.ByteWidth = sizeof(unsigned short) * comp.indices.size();
    sd.pSysMem = comp.indices.data();
    hr = m_gfx.GetDevice()->CreateBuffer(&bd, &sd, &comp.indexBuffer);
    if (FAILED(hr)) {
        std::stringstream ss;
        ss << "DX Error! HRESULT = 0x" << std::hex << hr;
        throw std::runtime_error(ss.str());
    }


    // 3) Load cubemap
    std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D>> tex(6);
    for (int i = 0;i < 6;i++) {
        Microsoft::WRL::ComPtr<ID3D11Resource> res;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tmp;
        hr = CreateWICTextureFromFile(
            m_gfx.GetDevice(), faces[i].c_str(), res.GetAddressOf(), tmp.GetAddressOf());
        if (FAILED(hr)) {
            std::stringstream ss;
            ss << "DX Error! HRESULT = 0x" << std::hex << hr;
            throw std::runtime_error(ss.str());
        }
        res.As(&tex[i]);
    }
    

    D3D11_TEXTURE2D_DESC desc;
    tex[0]->GetDesc(&desc);

    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.MipLevels = 1;
    desc.ArraySize = 6; 
    desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.Usage = D3D11_USAGE_DEFAULT; 
    desc.CPUAccessFlags = 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> cube;
    hr = m_gfx.GetDevice()->CreateTexture2D(&desc, nullptr, &cube);
    if (FAILED(hr)) {
        std::stringstream ss;
        ss << "DX Error! HRESULT = 0x" << std::hex << hr;
        throw std::runtime_error(ss.str());
    }
    for (UINT i = 0;i < 6;i++) {
        m_gfx.GetContext()->CopySubresourceRegion(
            cube.Get(), D3D11CalcSubresource(0, i, 1), 0, 0, 0, tex[i].Get(), 0, nullptr);
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
    srvd.Format = desc.Format;
    srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvd.TextureCube.MostDetailedMip = 0;
    srvd.TextureCube.MipLevels = 1;
    hr = m_gfx.GetDevice()->CreateShaderResourceView(cube.Get(), &srvd, &comp.cubeMapSRV);
    if (FAILED(hr)) {
        std::stringstream ss;
        ss << "DX Error! HRESULT = 0x" << std::hex << hr;
        throw std::runtime_error(ss.str());
    }

    // 4) Create sampler
    D3D11_SAMPLER_DESC sdsc{};
    sdsc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sdsc.AddressU = sdsc.AddressV = sdsc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sdsc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = m_gfx.GetDevice()->CreateSamplerState(&sdsc, &comp.samplerState);
    if (FAILED(hr)) {
        std::stringstream ss;
        ss << "DX Error! HRESULT = 0x" << std::hex << hr;
        throw std::runtime_error(ss.str());
    }
}
