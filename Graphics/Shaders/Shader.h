#pragma once
#pragma warning(push, 0)
#include <wrl.h>
#include <d3d11_4.h>
#include <string>
#pragma warning(pop)

using Microsoft::WRL::ComPtr;

namespace Graphics
{
	class GraphicsEngine;

	enum ShaderType
	{
		Compute,
		Domain,
		Geometry,
		Hull,
		Pixel,
		Vertex,
	};

	class Shader
	{
	public:
		ComPtr<ID3DBlob> ByteCode;

		Shader(GraphicsEngine& graphics, std::string path, ShaderType type);

		bool Create();
		void Use();
		void UnUse();
	private:
		ComPtr<ID3D11ComputeShader> ComputeShader;
		ComPtr<ID3D11DomainShader> DomainShader;
		ComPtr<ID3D11GeometryShader> GeometryShader;
		ComPtr<ID3D11HullShader> HullShader;
		ComPtr<ID3D11PixelShader> PixelShader;
		ComPtr<ID3D11VertexShader> VertexShader;
		GraphicsEngine& Graphics;
		ShaderType Type;

		Shader& operator=(const Shader& shader) = delete;
	};
}
