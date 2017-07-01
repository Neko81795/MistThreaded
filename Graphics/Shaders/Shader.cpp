#include "Shader.h"
#pragma warning(push, 0)
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <cstdlib>
#include "GraphicsEngine.h"
#pragma warning(pop)

namespace Graphics
{
	Shader::Shader(GraphicsEngine & graphics, std::string path, ShaderType type) : Graphics(graphics), Type(type)
	{
		std::wstring str;
		str.resize(path.size());

		mbstowcs(&str[0], &path[0], path.size());
		D3DReadFileToBlob(str.c_str(), ByteCode.GetAddressOf());
	}

	bool Shader::Create()
	{
		switch (Type)
		{
		case Graphics::Compute:
			if (FAILED(Graphics.Device->CreateComputeShader(ByteCode->GetBufferPointer(), ByteCode->GetBufferSize(), nullptr, ComputeShader.ReleaseAndGetAddressOf())))
				return false;
			break;
		case Graphics::Domain:
			if (FAILED(Graphics.Device->CreateDomainShader(ByteCode->GetBufferPointer(), ByteCode->GetBufferSize(), nullptr, DomainShader.ReleaseAndGetAddressOf())))
				return false;
			break;
		case Graphics::Geometry:
			if (FAILED(Graphics.Device->CreateGeometryShader(ByteCode->GetBufferPointer(), ByteCode->GetBufferSize(), nullptr, GeometryShader.ReleaseAndGetAddressOf())))
				return false;
			break;
		case Graphics::Hull:
			if (FAILED(Graphics.Device->CreateHullShader(ByteCode->GetBufferPointer(), ByteCode->GetBufferSize(), nullptr, HullShader.ReleaseAndGetAddressOf())))
				return false;
			break;
		case Graphics::Pixel:
			if (FAILED(Graphics.Device->CreatePixelShader(ByteCode->GetBufferPointer(), ByteCode->GetBufferSize(), nullptr, PixelShader.ReleaseAndGetAddressOf())))
				return false;
			break;
		case Graphics::Vertex:
			if (FAILED(Graphics.Device->CreateVertexShader(ByteCode->GetBufferPointer(), ByteCode->GetBufferSize(), nullptr, VertexShader.ReleaseAndGetAddressOf())))
				return false;
			break;
		}
		return true;
	}

	void Shader::Use()
	{
		switch (Type)
		{
		case Graphics::Compute:
			Graphics.DeviceContext->CSSetShader(ComputeShader.Get(), nullptr, 0);
			break;
		case Graphics::Domain:
			Graphics.DeviceContext->DSSetShader(DomainShader.Get(), nullptr, 0);
			break;
		case Graphics::Geometry:
			Graphics.DeviceContext->GSSetShader(GeometryShader.Get(), nullptr, 0);
			break;
		case Graphics::Hull:
			Graphics.DeviceContext->HSSetShader(HullShader.Get(), nullptr, 0);
			break;
		case Graphics::Pixel:
			Graphics.DeviceContext->PSSetShader(PixelShader.Get(), nullptr, 0);
			break;
		case Graphics::Vertex:
			Graphics.DeviceContext->VSSetShader(VertexShader.Get(), nullptr, 0);
			break;
		}
	}

	void Shader::UnUse()
	{
		switch (Type)
		{
		case Graphics::Compute:
			Graphics.DeviceContext->CSSetShader(nullptr, nullptr, 0);
			break;
		case Graphics::Domain:
			Graphics.DeviceContext->DSSetShader(nullptr, nullptr, 0);
			break;
		case Graphics::Geometry:
			Graphics.DeviceContext->GSSetShader(nullptr, nullptr, 0);
			break;
		case Graphics::Hull:
			Graphics.DeviceContext->HSSetShader(nullptr, nullptr, 0);
			break;
		case Graphics::Pixel:
			Graphics.DeviceContext->PSSetShader(nullptr, nullptr, 0);
			break;
		case Graphics::Vertex:
			Graphics.DeviceContext->VSSetShader(nullptr, nullptr, 0);
			break;
		}
	}
}


