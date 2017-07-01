#pragma once
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include <exception>
#include <memory>

namespace Graphics
{
	template<typename T>
	ConstantBuffer<T>::ConstantBuffer(GraphicsEngine& graphics, ShaderType type, UINT slot) : Graphics(graphics), Type(type), Slot(slot)
	{
		Create();
		Dirty = false;
	}

	template<typename T>
	template<typename ...Arguments>
	ConstantBuffer<T>::ConstantBuffer(GraphicsEngine & graphics, ShaderType type, UINT slot, Arguments ...constructorParams) 
		: 
		Graphics(graphics), 
		Type(type), 
		Slot(slot), 
		Data(constructorParams...)
	{
		Create();
		Dirty = false;
	}

	template<typename T>
	const T & ConstantBuffer<T>::GetData() const
	{
		return Data;
	}

	template<typename T>
	T & ConstantBuffer<T>::GetDataForWrite()
	{
		Dirty = true;
		return Data;
	}

	template<typename T>
	void ConstantBuffer<T>::SetData(const T & data)
	{
		Data = data;
		Dirty = true;
	}

	template<typename T>
	void ConstantBuffer<T>::Use()
	{
		if (Dirty)
		{
			if (!Update())
				throw std::exception("Failed to update ConstantBuffer");
		}
		switch (Type)
		{
		case ShaderType::Compute:
			Graphics.DeviceContext->CSSetConstantBuffers(Slot, 1, Buffer.GetAddressOf());
			break;
		case ShaderType::Domain:
			Graphics.DeviceContext->DSSetConstantBuffers(Slot, 1, Buffer.GetAddressOf());
			break;
		case ShaderType::Geometry:
			Graphics.DeviceContext->GSSetConstantBuffers(Slot, 1, Buffer.GetAddressOf());
			break;
		case ShaderType::Hull:
			Graphics.DeviceContext->HSSetConstantBuffers(Slot, 1, Buffer.GetAddressOf());
			break;
		case ShaderType::Pixel:
			Graphics.DeviceContext->PSSetConstantBuffers(Slot, 1, Buffer.GetAddressOf());
			break;
		case ShaderType::Vertex:
			Graphics.DeviceContext->VSSetConstantBuffers(Slot, 1, Buffer.GetAddressOf());
			break;
		}
	}

	template<typename T>
	bool ConstantBuffer<T>::Update()
	{
		//no need to update if the data isn't dirty
		if (!Dirty)
			return true;

		//lock access to the buffer on the GPU
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		if (FAILED(Graphics.DeviceContext->Map(Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			return false;
		//update the buffer
		memcpy(mappedResource.pData, &Data, sizeof(T));
		//let the GPU play with it again
		Graphics.DeviceContext->Unmap(Buffer.Get(), 0);
		
		Dirty = false;
		return true;
	}

	template<typename T>
	bool ConstantBuffer<T>::Create()
	{
		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = sizeof(T);
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;//Dynamic does not support CPU Read

		D3D11_SUBRESOURCE_DATA data{};
		data.pSysMem = &Data;

		if (FAILED(Graphics.Device->CreateBuffer(&desc, &data, Buffer.ReleaseAndGetAddressOf())))
			return false;
		return true;
	}
}