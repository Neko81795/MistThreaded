#pragma once
#pragma warning(push, 0)
#include <d3d11_4.h>
#include <wrl.h>
#include <vector>
#pragma warning(pop)

using Microsoft::WRL::ComPtr;

namespace Graphics
{
	class GraphicsEngine;
	class Shader;

	struct Mesh
	{
	public:
		struct Vertex
		{
			float Position[3];
			float Color[4];
		};

		ComPtr<ID3D11Buffer> VertBuffer;
		ComPtr<ID3D11Buffer> IndexBuffer;
		ComPtr<ID3D11InputLayout> Layout;

		Mesh(GraphicsEngine& graphics, const std::vector<Vertex>& vertices, const std::vector<unsigned short>& indices);

		void Use() const;
		bool Create(const Shader& shader);
		bool IsMutable() const;
		void SetMutable(bool mute);

		const std::vector<Vertex>& GetVertices() const;
		const std::vector<unsigned short>& GetIndices() const;
		void SetVertex(size_t index, const Vertex& vertex);
		void SetIndex(size_t index, unsigned short value);

		Mesh Copy() const;
	private:
		GraphicsEngine* Graphics;
		const Shader*pShader;
		bool Mutable;
		bool Created;
		bool VDirty;
		bool IDirty;
		std::vector<Vertex> Vertices;
		std::vector<unsigned short> Indices;

		bool UpdateVerices() const;
		bool UpdateIndices() const;
	};
}
