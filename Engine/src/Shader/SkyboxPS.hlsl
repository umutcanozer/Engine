TextureCube skybox : register(t0);
SamplerState samp : register(s0);

struct PS_INPUT {
    float4 pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET{
    return skybox.Sample(samp, normalize(input.texCoord));
}
