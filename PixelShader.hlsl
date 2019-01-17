// Texture, aka Color Map
Texture2D ObjTexture;
SamplerState ObjSamplerState;

//SamplerState linearSampler
//{
//	Filter = min_mag_mip_linear;
//	AddressU = Clamp;
//	AddressV = Clamp;
//	MaxAnisotropy = 16;
//};

cbuffer cbPerObj
{
	float4x4 WVP;
	float2 Dimensions;
	bool HasTexture;
	bool ShouldClip;
	float4 Color;
};


struct PS_Input
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

float4 main(PS_Input input) : SV_TARGET
{
	//float4 color = input.Color; // colorMap.Sample(ObjSamplerState, input.TexCoord);
	////color[3] = min(color[3], input.Opacity);
	//return color;

	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);

	if (HasTexture)
	{
		if(ShouldClip)
			clip(diffuse.a - 0.25);
		return diffuse;
	}
	else
		return Color;
}