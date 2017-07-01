#include "Texture.h"

#include "GraphicsEngine.h"


namespace Graphics
{
	Texture::Texture(Graphics::GraphicsEngine& graphics, std::wstring path) : Graphics(graphics), Path(path), Length(0)
	{
		//start er up
		ULONG_PTR gdiplustoken;
		Gdiplus::GdiplusStartupInput inp;
		Gdiplus::GdiplusStartup(&gdiplustoken, &inp, nullptr);
		Gdiplus::Bitmap bitmap(path.c_str());

		if (bitmap.GetLastStatus() == Gdiplus::Status::InvalidParameter)
			throw std::exception("Failed to load image");

		//get image info
		Width = bitmap.GetWidth();
		Height = bitmap.GetHeight();
		Gdiplus::PixelFormat Format = bitmap.GetPixelFormat();

		//convert to ARGB if needed
		if (Format != PixelFormat32bppARGB)
			bitmap.ConvertFormat(PixelFormat32bppARGB, Gdiplus::DitherType::DitherTypeNone, Gdiplus::PaletteType::PaletteTypeCustom, nullptr, 0);
		Format = bitmap.GetPixelFormat();

		//get the data
		Gdiplus::Rect rect(0, 0, static_cast<int>(Width), static_cast<int>(Height));
		Gdiplus::BitmapData data;


		if (Gdiplus::Ok == bitmap.LockBits(&rect, Gdiplus::ImageLockModeRead, Format, &data))
		{
			Stride = static_cast<UINT>(std::abs(data.Stride));
			Length = data.Height * Stride;
			Bytes.resize(Length);

			memcpy(&Bytes[0], data.Scan0, Length);

			bitmap.UnlockBits(&data);
		}
		else
		{
			throw std::exception("NOT SURE WHAT HAPPEN");
		}
	}

	void Texture::Create()
	{
		//create texture
		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.Width = Width;
		texDesc.Height = Height;
		texDesc.MipLevels = 1;

		texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;

		texDesc.ArraySize = 1;
		texDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.SampleDesc.Count = 1;

		D3D11_SUBRESOURCE_DATA data{};
		data.pSysMem = Bytes.data();
		data.SysMemPitch = Stride;

		HRESULT hr = Graphics.Device->CreateTexture2D(&texDesc, &data, Tex.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			return;
		
		//create shader resource
		Graphics.Device->CreateShaderResourceView(Tex.Get(), nullptr, Resource.ReleaseAndGetAddressOf());

		//create sampler
		D3D11_SAMPLER_DESC samplerDesc{};

		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

		Graphics.Device->CreateSamplerState(&samplerDesc, Sampler.ReleaseAndGetAddressOf());
	}

	void Texture::Use()
	{
		Graphics.DeviceContext->PSSetSamplers(0, 1, Sampler.GetAddressOf());

		Graphics.DeviceContext->PSSetShaderResources(0, 1, Resource.GetAddressOf());
	}
}

