#include "GraphicsEngine.h"
#include <exception>
#include <vector>
#include "Mesh.h"

namespace Graphics
{
	static GraphicsEngine* GlobalEngine;

	void Res()
	{
		GlobalEngine->DeviceContext->ClearState();
		GlobalEngine->RenderTargetView.Reset();
		GlobalEngine->DepthStencilView.Reset();

		HRESULT res = GlobalEngine->SwapChain->ResizeBuffers(0, GlobalEngine->Win.Width, GlobalEngine->Win.Height, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		if (FAILED(res))
			GlobalEngine->CreateDeviceDependentResources();
		else
			GlobalEngine->CreateBackBuffer();
	}

	GraphicsEngine::GraphicsEngine(Window& win, bool CreateForDebug) : Win(win)
	{
		UINT flag = 0;
		if (CreateForDebug)
			flag = DXGI_CREATE_FACTORY_DEBUG;

		//create the factory
		if (FAILED(CreateDXGIFactory2(flag, IID_PPV_ARGS(&Factory))))
			throw std::exception("Failed to create DXGI Factory");


		//now we initialize DX

		//specify the DX levels we will accept
		D3D_FEATURE_LEVEL featureLevels[] = 
		{
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0
		};

		//create the device
		UINT deviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_SINGLETHREADED;
		if (CreateForDebug)
			deviceFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG
			| D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUGGABLE; //forces software render

		ComPtr<ID3D11Device> dev;
		if (FAILED(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			deviceFlags,
			featureLevels,
			_ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			dev.GetAddressOf(),
			&FeatureLevel,
			nullptr)))
		{
			//no dice. lets try for a warp device
			if (FAILED(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_WARP,
				nullptr,
				deviceFlags,
				featureLevels,
				_ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				dev.GetAddressOf(),
				&FeatureLevel,
				nullptr)))
			{
				throw std::exception("Failed to create device");
			}
			else
				RenderingInSoftware = true;
		}

		//we have a device, but lets make it a device4
		if (FAILED(dev.As(&Device)))
			throw std::exception("Failed to create device");

		Device->GetImmediateContext3(DeviceContext.GetAddressOf());

		if (!GetRefreshRate())
			throw std::exception("Failed to poll refresh rate");

		if (!CreateDeviceDependentResources())
			throw std::exception("Failed to create device dependent resources");

		GlobalEngine = this;

		Win.onResize = Res;
	}

	bool GraphicsEngine::CreateDeviceDependentResources()
	{
		if (!CreateSwapChain())
			return false;

		if (!CreateBackBuffer())
			return false;

		SetBlendMode(BlendMode::Multiply);

		return true;
	}

	void GraphicsEngine::ClearScreen(const Color & color)
	{
		DeviceContext->ClearRenderTargetView(RenderTargetView.Get(), color);
		DeviceContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);

		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		ViewPort.Height = static_cast<float>(Win.Height);
		ViewPort.Width = static_cast<float>(Win.Width);
		ViewPort.MaxDepth = 1.0f;
		ViewPort.MinDepth = 0.0f;
		DeviceContext->RSSetViewports(1, &ViewPort);
	}

	void GraphicsEngine::SetBlendMode(BlendMode blendMode)
	{
		D3D11_BLEND_DESC1 blendStateDesc{};

		blendStateDesc.RenderTarget[0].BlendEnable = true;
		switch (blendMode)
		{
		case Graphics::GraphicsEngine::Multiply:
			blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			break;
		case Graphics::GraphicsEngine::Additive:
			blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
			blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
			blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			break;
		case Graphics::GraphicsEngine::Subtractive:
			blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
			blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
			blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_REV_SUBTRACT;
			break;
		}

		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		Device->CreateBlendState1(&blendStateDesc, BlendState.ReleaseAndGetAddressOf());
		DeviceContext->OMSetBlendState(BlendState.Get(), nullptr, 0xffffffff);
	}

	void GraphicsEngine::Draw(const Mesh& mesh)
	{
		mesh.Use();
		DeviceContext->DrawIndexed(static_cast<UINT>(mesh.GetIndices().size()), 0, 0);
	}

	void GraphicsEngine::Present()
	{
		switch (VSync)
		{
		case GraphicsEngine::On:
			SwapChain->Present(1, 0);
			break;
		case GraphicsEngine::Half:
			SwapChain->Present(2, 0);
			break;
		case GraphicsEngine::Off:
		default:
			SwapChain->Present(0, 0);
			break;
		}
	}

	bool GraphicsEngine::GetRefreshRate()
	{
		ComPtr<IDXGIAdapter1> adapt;
		//get the primary adapter
		if (FAILED(Factory->EnumAdapters1(0, adapt.GetAddressOf())))
			return false;
		//try to convert it to a adapter3
		if (FAILED(adapt.As(&Adapter)))
			return false;

		//get an output
		ComPtr<IDXGIOutput> adapterOutput;
		if (FAILED(Adapter->EnumOutputs(0, adapterOutput.GetAddressOf())))
			return false;

		//get the number of output modes
		UINT numModes;
		if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr)))
			return false;

		std::vector<DXGI_MODE_DESC> displayModeList;
		displayModeList.resize(numModes);

		//for real this time
		if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, &displayModeList[0])))
			return false;

		//and then just take the first one because I'm lazy
		RefreshRate = displayModeList[0].RefreshRate;


		//clear the comptr we don't need
		adapterOutput.Reset();
		return true;
	}
	
	bool GraphicsEngine::CreateSwapChain()
	{
		//describe the swap chain we want
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDesc = {};

		swapChainDesc.BufferCount = 1; //single buffered
		swapChainDesc.Width = Win.Width;
		swapChainDesc.Height = Win.Height;
		swapChainDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapChainFullScreenDesc.RefreshRate = RefreshRate;
		swapChainFullScreenDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_CENTERED;
		swapChainFullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainFullScreenDesc.Windowed = true; // start windowed

		//no multi-sampling
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

		//get the swap chain
		ComPtr<IDXGISwapChain1> swap;
		if (FAILED(Factory->CreateSwapChainForHwnd(Device.Get(), Win.Handle, &swapChainDesc, &swapChainFullScreenDesc, nullptr, swap.GetAddressOf())))
			return false;

		//now make it a swap chain3
		SwapChain.Reset();
		if (FAILED(swap.As(&SwapChain)))
			return false;
		swap.Reset();

		return true;
	}
	
	bool GraphicsEngine::CreateBackBuffer()
	{
		//get the backBuffer
		ComPtr<ID3D11Texture2D> backBuffer;

		if (FAILED(SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))))
			return false;

		//create the render target view
		if (FAILED(Device->CreateRenderTargetView(backBuffer.Get(), nullptr, RenderTargetView.ReleaseAndGetAddressOf())))
			return false;


		//describe the depth stencil buffer
		D3D11_TEXTURE2D_DESC depthBufferDesc{};
		depthBufferDesc.Width = Win.Width;
		depthBufferDesc.Height = Win.Height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0; //no access
		depthBufferDesc.MiscFlags = 0;

		//create the depth stencil buffer
		if (FAILED(Device->CreateTexture2D(&depthBufferDesc, nullptr, DepthStencilBuffer.ReleaseAndGetAddressOf())))
			return false;

		//describe the depth stencil state
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		//what to do when we fail stencil testing?
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		//what to do when we pass stencil testing, but fail depth testing?
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
		//what to do when both pass?
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;

		//what to do when we fail stencil testing?
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		//what to do when we pass stencil testing, but fail depth testing?
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR;
		//what to do when both pass?
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;

		//create the depth stencil state
		if (FAILED(Device->CreateDepthStencilState(&depthStencilDesc, DepthStencilState.ReleaseAndGetAddressOf())))
			return false;
		
		//set the state
		DeviceContext->OMSetDepthStencilState(DepthStencilState.Get(), 1);

		//describe the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};

		depthStencilViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		//create the depth stencil view
		if (FAILED(Device->CreateDepthStencilView(DepthStencilBuffer.Get(), &depthStencilViewDesc, DepthStencilView.ReleaseAndGetAddressOf())))
			return false;


		//set the render target
		DeviceContext->OMSetRenderTargets(1, RenderTargetView.GetAddressOf(), DepthStencilView.Get());

		return true;
	}
}
