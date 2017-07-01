#pragma once
#pragma warning(push, 0)
#include <wrl.h>
#include <d3d11_4.h>
#include "Shaders/Shader.h"
#pragma warning(pop)

namespace Graphics
{
	class GraphicsEngine;

	template <typename T>
	class ConstantBuffer
	{
	public:

		Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer;
		GraphicsEngine& Graphics;
		UINT Slot;
		
		/// <summary>
		/// Creates a constant buffer for the given shader type in the given slot
		/// </summary>
		/// <param name="graphics">the graphics engine</param>
		/// <param name="type">the type of shader that will use it</param>
		/// <param name="slot">the slot of the buffer</param>
		ConstantBuffer(GraphicsEngine& graphics, ShaderType type, UINT slot);

		/// <summary>
		/// Creates a constant buffer for the given shader type in the given slot
		/// initialized with the given parameters
		/// </summary>
		/// <param name="graphics">the graphics engine</param>
		/// <param name="type">the type of shader that will use it</param>
		/// <param name="slot">the slot of the buffer</param>
		/// <param name="constructorParams">the parameters for constructing the data</param>
		template<typename... Arguments>
		ConstantBuffer(GraphicsEngine& graphics, ShaderType type, UINT slot, Arguments... constructorParams);
		
		/// <summary>
		/// Gets a read-only reference to the data
		/// </summary>
		const T& GetData() const;
		/// <summary>
		/// Gets a reference to the data that can be written to.
		/// it is assumed that the data will be written to
		/// so, the data will be sent to the GPU when it's used
		/// </summary>
		T& GetDataForWrite();
		/// <summary>
		/// Sets the data in the buffer to the given value
		/// </summary>
		/// <param name="data">the data to set the buffer to</param>
		void SetData(const T& data);
		/// <summary>
		/// Tells the GPU to use the data and updates the GPU's copy if needed
		/// </summary>
		void Use();

		/// <summary>
		/// Updates the data on the GPU
		/// </summary>
		/// <returns>If the operation was successful</returns>
		bool Update();
	private:
		T Data;
		bool Dirty;
		ShaderType Type;

		ConstantBuffer& operator=(const ConstantBuffer& rhs) = delete;

		bool Create();
	};
}

#include "ConstantBuffer.hpp"
