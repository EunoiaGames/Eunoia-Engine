struct VSInput
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.pos = float4(input.pos.x, input.pos.y, input.pos.z, 1.0);
	output.texCoord = input.texCoord;
	return output;
}

Texture2D Albedo;
SamplerState AlbedoState;

float4 PSMain(VSOutput input) : SV_TARGET0
{
	return Albedo.Sample(AlbedoState, input.texCoord);
}