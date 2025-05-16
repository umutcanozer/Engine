cbuffer CameraBuffer : register(b0)
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

    // Kameranýn dönüþünü al, pozisyonunu yok et
    float4x4 viewNoTranslation = viewMatrix;
    viewNoTranslation._41 = 0.0f;
    viewNoTranslation._42 = 0.0f;
    viewNoTranslation._43 = 0.0f;

    // Pozisyonu hesapla
    float4 worldPos = mul(float4(input.pos, 1.0f), viewNoTranslation);
    float4 clipPos = mul(worldPos, projMatrix);
    output.pos = clipPos;

    // Texture coordinate cube map için normalize edilmiþ poz
    output.texCoord = input.pos;

    return output;
}
