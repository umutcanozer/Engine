#include "Graphics.h"

Graphics::Graphics(HWND hWnd, int width, int height)
{
	HRESULT hr = S_OK;
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

#pragma region device
	hr = D3D11CreateDevice(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		deviceFlags, &featureLevel, 1,
		D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext
	);

	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scd.Flags = 0;

	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	m_pDevice.As(&dxgiDevice); 

	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(dxgiAdapter.GetAddressOf()));

	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));

	dxgiFactory->CreateSwapChain(m_pDevice.Get(), &scd, &m_pSwapChain);

#pragma endregion

#pragma region rendertarget/depthstencil
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_backBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &p_backBuffer);
	hr = m_pDevice->CreateRenderTargetView(p_backBuffer.Get(), nullptr, &m_pRenderTargetView);
	if (FAILED(hr))
	{
		std::cerr << "CreateRenderTargetView failed with error: " << hr << std::endl;
		return;
	}
	D3D11_TEXTURE2D_DESC backBufferDesc = {};
	p_backBuffer->GetDesc(&backBufferDesc);


	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_depthBuffer;
	D3D11_TEXTURE2D_DESC dDesc = {};
	dDesc.Width = backBufferDesc.Width;
	dDesc.Height = backBufferDesc.Height;
	dDesc.MipLevels = 1u;
	dDesc.ArraySize = 1u;
	dDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dDesc.SampleDesc.Count = 1u;
	dDesc.SampleDesc.Quality = 0u;
	dDesc.Usage = D3D11_USAGE_DEFAULT;
	dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = m_pDevice->CreateTexture2D(&dDesc, nullptr, &p_depthBuffer);
	if (FAILED(hr))
	{
		std::cerr << "CreateTexture2D failed with error: " << hr << std::endl;
		return;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsViewDesc = {};
	dsViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsViewDesc.Texture2D.MipSlice = 0u;

	hr = m_pDevice->CreateDepthStencilView(p_depthBuffer.Get(), &dsViewDesc, &m_pDepthStencilView);
	if (FAILED(hr))
	{
		std::cerr << "CreateDepthStencilView failed with error: " << hr << std::endl;
		return;
	}

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> p_depthSState;
	hr = m_pDevice->CreateDepthStencilState(&dsDesc, &p_depthSState);
	if (FAILED(hr))
	{
		std::cerr << "CreateDepthStencilState failed with error: " << hr << std::endl;
		return;
	}

	m_pDeviceContext->OMSetDepthStencilState(p_depthSState.Get(), 1u);
#pragma endregion
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	m_pDeviceContext->RSSetViewports(1, &vp);	
}

void Graphics::BeginFrame(float red, float green, float blue, float alpha = 1.0f)
{
	const float color[] = { red, green, blue, alpha };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
}

void Graphics::EndFrame()
{
	HRESULT hr = m_pSwapChain->Present(1u, 0u);
	if (FAILED(hr))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			throw m_pDevice->GetDeviceRemovedReason();
		}
		else
		{
			std::stringstream ss;
			ss << "Present failed with error: 0x" << std::hex << hr;
			throw std::runtime_error(ss.str());
		}
	}
}

Graphics::~Graphics()
{
	if (m_pDeviceContext)
		m_pDeviceContext->ClearState();
}
