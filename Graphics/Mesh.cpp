#include "Mesh.h"
#include "GraphicsEngine.h"
#include "Shaders/Shader.h"

namespace Graphics
{
	Mesh::Mesh(GraphicsEngine& graphics, const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices)
		: Graphics(&graphics),
		Vertices(vertices),
		Indices(indices),
		Mutable(false),
	    Created(false),
		VDirty(false),
		IDirty(false)
	{
	}

	void Mesh::Use() const
	{
		if (!Created)
			return;//todo Warn someone?

		if (Mutable)
		{
			if (VDirty)
			{
				UpdateVerices();
			}
			if (IDirty)
			{
				UpdateIndices();
			}
		}

		ID3D11DeviceContext3* dc = Graphics->DeviceContext.Get();
		dc->IASetInputLayout(Layout.Get());
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		dc->IASetVertexBuffers(0, 1, VertBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//16-bit Unsigned integers (unsigned short)
		dc->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	}

	bool Mesh::Create(const Shader& shader)
	{
		pShader = &shader;
		//define the vertex buffer
		D3D11_BUFFER_DESC vbDesc{};
		vbDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * Vertices.size());
		vbDesc.StructureByteStride = sizeof(Vertex);
		vbDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

		if (Mutable)
		{
			vbDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
		else
			vbDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA vbData{};
		vbData.pSysMem = Vertices.data();

		//create the vertex buffer
		if (FAILED(Graphics->Device->CreateBuffer(&vbDesc, &vbData, VertBuffer.ReleaseAndGetAddressOf())))
			return false;

		//define the index buffer
		D3D11_BUFFER_DESC ibDesc{};
		ibDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * Vertices.size());
		ibDesc.StructureByteStride = sizeof(Vertex);
		ibDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

		if (Mutable)
		{
			ibDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
			ibDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		}
		else
			ibDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA ibData{};
		ibData.pSysMem = Indices.data();

		//create the index buffer
		if (FAILED(Graphics->Device->CreateBuffer(&ibDesc, &ibData, IndexBuffer.ReleaseAndGetAddressOf())))
			return false;

		//create the layout
		D3D11_INPUT_ELEMENT_DESC layoutDesc[] = 
		{
			/*name       index             format           input slot      alignedByteOffset          InputSlotClass             InstanceDataStepRate*/
			{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		if (FAILED(Graphics->Device->CreateInputLayout(layoutDesc, _ARRAYSIZE(layoutDesc), shader.ByteCode->GetBufferPointer(), shader.ByteCode->GetBufferSize(), Layout.ReleaseAndGetAddressOf())))
			return false;

		Created = true;
		return true;
	}

	bool Mesh::IsMutable() const
	{
		return Mutable;
	}

	void Mesh::SetMutable(bool mute)
	{
		if (mute == Mutable)
			return;

		Mutable = mute;
		if (Created)
		{
			//destroy what we have because it doesn't match
			VertBuffer.Reset();
			IndexBuffer.Reset();
			Layout.Reset();
			Created = false;
			Create(*pShader);
		}
	}

	const std::vector<Mesh::Vertex>& Mesh::GetVertices() const
	{
		return Vertices;
	}

	const std::vector<unsigned short>& Mesh::GetIndices() const
	{
		return Indices;
	}

	void Mesh::SetVertex(size_t index, const Vertex & vertex)
	{
		Vertices[index] = vertex;
		VDirty = true;
	}

	void Mesh::SetIndex(size_t index, unsigned short value)
	{
		Indices[index] = value;
		IDirty = true;
	}
	
	Mesh Mesh::Copy() const
	{
		Mesh ret(*Graphics, Vertices, Indices);
		ret.SetMutable(Mutable);
		if (Created)
			ret.Create(*pShader);

		return ret;
	}

	bool Mesh::UpdateVerices() const
	{
		//lock access to the buffer on the GPU
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		HRESULT res = Graphics->DeviceContext->Map(VertBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(res))
			return false;

		//update the buffer
		memcpy(mappedResource.pData, Vertices.data(), sizeof(Vertex) * Vertices.size());

		//let the GPU play with it again
		Graphics->DeviceContext->Unmap(VertBuffer.Get(), 0);

		const_cast<Mesh*>(this)->VDirty = false;
		return true;
	}

	bool Mesh::UpdateIndices() const
	{
		//lock access to the buffer on the GPU
		D3D11_MAPPED_SUBRESOURCE mappedResource{};
		if (FAILED(Graphics->DeviceContext->Map(IndexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			return false;

		//update the buffer
		memcpy(mappedResource.pData, Indices.data(), sizeof(unsigned short) * Indices.size());

		//let the GPU play with it again
		Graphics->DeviceContext->Unmap(IndexBuffer.Get(), 0);

		const_cast<Mesh*>(this)->IDirty = false;
		return true;
	}
}
