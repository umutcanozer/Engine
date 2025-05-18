cbuffer MatrixBuffer : register(b0)
{
	float4x4 worldMatrix;
};

cbuffer CameraBuffer : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projMatrix;
};

struct VSInput {
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
	float4 color	: COLOR;
	float3 normal   : NORMAL;
};

struct PSInput {
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color	: COLOR;
	float3 worldPos : POSITION1;
	float3 normal   : NORMAL;
};

PSInput main(VSInput input) {
	PSInput output;
	float4 worldPos = mul(float4(input.position, 1.0f), worldMatrix);
	float4 viewPos = mul(worldPos, viewMatrix);
	output.position = mul(viewPos, projMatrix);

	output.texcoord = input.texcoord;
	output.color = input.color;
	output.worldPos = worldPos.xyz;

	float3 transformedNormal = mul(float4(input.normal, 0.0f), worldMatrix).xyz;
	output.normal = normalize(transformedNormal);

	return output;
}