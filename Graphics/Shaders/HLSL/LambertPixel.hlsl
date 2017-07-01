#include "Lambert.hlsli"
Texture2D Color : register(t0);
SamplerState Sampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 color = input.color * Color.Sample(Sampler, input.UV);
	clip(color.a - 0.01);
	return color;
}
