#pragma once

#pragma warning(push, 0)
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi1_5.h>
#include <d3dcommon.h>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>
#include "Window.h"
#include "Color.h"
#pragma warning(pop)

using Microsoft::WRL::ComPtr;

namespace Graphics
{
	struct Mesh;

	class GraphicsEngine
	{
		static friend void Res();
	public:
		enum VSyncType
		{
			Off,
			On,
			Half
		};
		enum BlendMode
		{
			Multiply,
			Additive,
			Subtractive
		};

		ComPtr<ID3D11Device4> Device;
		ComPtr<ID3D11DeviceContext3> DeviceContext;
		ComPtr<ID3D11RasterizerState2> RasterState;
		ComPtr<IDXGIFactory4> Factory;
		ComPtr<IDXGIAdapter3> Adapter;
		DXGI_RATIONAL RefreshRate;
		bool RenderingInSoftware;

		ComPtr<IDXGISwapChain3> SwapChain;
		ComPtr<ID3D11RenderTargetView> RenderTargetView;
		ComPtr<ID3D11Texture2D> DepthStencilBuffer;
		ComPtr<ID3D11DepthStencilState> DepthStencilState;
		ComPtr<ID3D11DepthStencilView> DepthStencilView;
		ComPtr<ID3D11BlendState1> BlendState;
		D3D11_VIEWPORT ViewPort;
		Window& Win;
		VSyncType VSync;

		GraphicsEngine(Window& win, bool CreateForDebug = false);

		bool CreateDeviceDependentResources();
		void ClearScreen(const Color& col = Color(0, 0.5f, 1, 1));
		void SetBlendMode(BlendMode blendMode);
		void Draw(const Mesh& mesh);
		void Present();

	private:
		D3D_FEATURE_LEVEL FeatureLevel;

		GraphicsEngine& operator=(const GraphicsEngine& rhs) = delete;

		bool GetRefreshRate();
		bool CreateSwapChain();
		bool CreateBackBuffer();
	};
}
