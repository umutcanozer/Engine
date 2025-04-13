#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>
#include <sstream>

class Graphics
{
public:
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete; 
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void BeginFrame(float red, float green, float blue, float alpha);
	void EndFrame();

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
};


