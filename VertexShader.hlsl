cbuffer cbPerObject
{
	float4x4 WVP;
	float2 Dimensions : DIMENSIONS;
	bool HasTexture;
	float4 Color;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD) // in  | VS_INPUT input
{

	VS_OUTPUT output;

	output.Pos = inPos; // mul(inPos, WVP); // inPos;
	// mul(input.Pos, WVP); //  inPos
	// output.Color = inColor;
	output.TexCoord = inTexCoord;
	// output.Opacity = input.Opacity;

	return output;
}