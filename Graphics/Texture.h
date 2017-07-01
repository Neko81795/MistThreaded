#pragma once

#pragma warning(push, 0)
#include <d3d11_4.h>
#include <wrl.h>
#define GDIPVER 0x0110
#include <gdiplus.h>
#include <string>
#include <vector>
#pragma comment(lib, "Gdiplus.lib")
#pragma warning(pop)

using Microsoft::WRL::ComPtr;

namespace Graphics
{
	class GraphicsEngine;

	class Texture
	{
	public:
		Texture(GraphicsEngine& engine, std::wstring path);

		void Create();
		void Use();
	private:
		GraphicsEngine& Graphics;
		ComPtr<ID3D11Texture2D> Tex;
		ComPtr<ID3D11SamplerState> Sampler;
		ComPtr<ID3D11ShaderResourceView> Resource;
		UINT Width;
		UINT Height;
		UINT Stride;
		size_t Length;
		std::wstring Path;
		std::vector<BYTE> Bytes;
		
		Texture& operator=(const Texture& other) = delete;
	};
}