cbuffer CameraBuffer : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projMatrix;
};

struct VS_INPUT {
    float3 pos : POSITION;
};

struct PS_INPUT {
    float4 pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;

    // View matrix'in translation'ýný sýfýrla (sabit kalacak)
    float4x4 viewNoTrans = viewMatrix;
    viewNoTrans._41 = 0.0f;
    viewNoTrans._42 = 0.0f;
    viewNoTrans._43 = 0.0f;

    float4 pos = mul(float4(input.pos, 1.0f), viewNoTrans);
    pos = mul(pos, projMatrix);

    // Z'yi uzaða sabitle, her þeyin arkasýnda kalsýn
    output.pos = float4(pos.xy, pos.w * 0.999f, pos.w);

    // CubeMap için normalize yön
    output.texCoord = input.pos;

    return output;
}
 