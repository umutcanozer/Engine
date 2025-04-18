Texture2D tex : register(t0);
SamplerState samp : register(s0);

struct PSInput {
    float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};


float4 main(PSInput input) : SV_TARGET{
	float4 texColor = tex.Sample(samp, input.texcoord);
    return texColor * input.color; 
}