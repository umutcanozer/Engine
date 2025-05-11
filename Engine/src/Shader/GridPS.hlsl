cbuffer GridCameraBuffer : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projMatrix;
    float3 cameraPosition;
    float padding; // Padding to align to 16 bytes
};

struct GridVSOutput {
    float4 position : SV_POSITION;
    float3 worldPos : TEXCOORD0;
};

float4 main(GridVSOutput input) : SV_TARGET
{
    float2 uv = input.worldPos.xz * 0.5f; 
    float2 grid = abs(frac(uv+0.5f) - 0.5f);   
    float gridLine = min(grid.x, grid.y);

    float thickness = 0.04f;
    float alpha = 1.0 - smoothstep(0.0, thickness, gridLine);

    float distance = length(input.worldPos - cameraPosition);
    float fade = saturate(1.0 - distance / 100.0); 

    float finalAlpha = alpha * fade;

    float axisThickness = 0.03f;
    float isXAxis = step(abs(input.worldPos.z), axisThickness);
    float isZAxis = step(abs(input.worldPos.x), axisThickness);
    float3 baseColor = float3(0.8f, 0.8f, 0.8f);
    float3 axisColor = lerp(baseColor, float3(1.0f, 0.0f, 0.0f), isXAxis);
    axisColor = lerp(axisColor, float3(0.0f, 0.0f, 1.0f), isZAxis);     

    float3 gridColor = float3(0.8f, 0.8f, 0.8f); 
    return float4(axisColor, finalAlpha);
}