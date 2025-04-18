cbuffer MatrixBuffer : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;
};

struct VSInput {
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float4 color	: COLOR;
};

struct PSInput {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color	: COLOR;
};

PSInput main(VSInput input) {
	PSInput output;
	float4 pos = float4(input.position, 1.0f);

	float4 worldPos = mul(pos, worldMatrix);
	float4 viewPos = mul(worldPos, viewMatrix);
	float4 projPos = mul(viewPos, projMatrix);
	output.position = projPos;
	output.texcoord = input.texcoord;
	output.color = input.color;
	return output;
}