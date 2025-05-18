Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer LightBuffer : register(b2)
{
    float3 lightPos;
    float range;
    float3 attenuation;
    float intensity;
    float4 ambient;
    float4 diffuse;
};

struct PSInput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    float3 worldPos : POSITION1;
    float3 normal : NORMAL;
};

float4 main(PSInput input) : SV_TARGET
{
    //Normalize normal
    float3 normal = normalize(input.normal);

    //Sample texture color and apply vertex color
    float4 texColor = tex.Sample(samp, input.texcoord) * input.color;

    //Light vector from light position to fragment position
    float3 lightToPixel = lightPos - input.worldPos;

    //Distance between pixel and light
    float d = length(lightToPixel);

    //Ambient light contribution
    float3 finalAmbient = texColor.rgb * ambient.rgb;

    //If pixel is out of range just return ambient
    if (d > range)
        return float4(finalAmbient, texColor.a);

    //Normalize light vector
    lightToPixel = normalize(lightToPixel);

    //Dot product for diffuse lighting (how directly the light hits the surface)
    float NdotL = dot(normal, lightToPixel);

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    if (NdotL > 0.0f)
    {
        float att = 1.0 / (attenuation.x + attenuation.y * d + attenuation.z * d * d);
        finalColor += NdotL * texColor.rgb * diffuse.rgb * att * intensity;
    }

    finalColor = saturate(finalAmbient + finalColor);
     
    return float4(finalColor, texColor.a);
}
