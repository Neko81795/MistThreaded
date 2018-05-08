#include "Lambert.hlsli"

struct VertexShaderInput
{
	float3 position : POSITION;
	float4 color : COLOR;
};

cbuffer ViewProjectionConstantBuffer : register(b0)
{
	matrix view;
	matrix viewProjection;
};

cbuffer ModelConstantBuffer : register(b1)
{
	matrix model;
}

PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;

	float4 pos = float4(input.position, 1.0f);
	pos = mul(model, pos);
	pos = mul(viewProjection, pos);
	output.position = pos;
	output.color = input.color;
	output.UV.x = input.position.x + 0.5;
	output.UV.y = input.position.y + 0.5;
	return output;
}
