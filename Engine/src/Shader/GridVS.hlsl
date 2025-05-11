cbuffer GridMatrixBuffer : register(b0)
{
    float4x4 worldMatrix;
};

cbuffer GridCameraBuffer : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projMatrix;
    float3 cameraPosition;
	float padding; // Padding to align to 16 bytes
};

struct GridVSInput {
    float3 position : POSITION;
};

struct GridVSOutput {
    float4 position : SV_POSITION;
    float3 worldPos : TEXCOORD0;
};

GridVSOutput main(GridVSInput input)
{
    GridVSOutput output;

    float4 world = mul(float4(input.position, 1.0f), worldMatrix);
    output.worldPos = world.xyz;

    float4 view = mul(world, viewMatrix);
    output.position = mul(view, projMatrix);

    return output;
}