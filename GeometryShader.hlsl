cbuffer cbPerObj
{
	float4x4 WVP;
	float2 Dimensions : DIMENSIONS;
};

struct GS_Output
{
	float4 Pos : SV_POSITION;
	//float  Opacity : OPACITY;
	float4 Color : COLOR;
};

[maxvertexcount(6)]
void main(point GS_Output quadCenter[1], inout TriangleStream <GS_Output> triStream)
{
	GS_Output output;
	output.Color = quadCenter[0].Color;
	// output.Opacity = quadCenter[0].Opacity;
	float oldZ = quadCenter[0].Pos.z;
	// create the quad //

	// top left
	output.Pos = float4(quadCenter[0].Pos.x - (Dimensions.x / 2), quadCenter[0].Pos.y + (Dimensions.y / 2), oldZ, 1);
	output.Pos = mul(output.Pos, WVP);
	//output.TexCoord = float2(0, 0);
	//output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	triStream.Append(output);

	// top right
	output.Pos = float4(quadCenter[0].Pos.x + (Dimensions.x / 2), quadCenter[0].Pos.y + (Dimensions.y / 2), oldZ, 1);
	output.Pos = mul(output.Pos, WVP);
	//output.TexCoord = float2(1, 0);
	//output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	triStream.Append(output);

	// bottom left
	output.Pos = float4(quadCenter[0].Pos.x - (Dimensions.x / 2), quadCenter[0].Pos.y - (Dimensions.y / 2), oldZ, 1);
	output.Pos = mul(output.Pos, WVP);
	//output.TexCoord = float2(0, 1);
	//output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	triStream.Append(output);

	// bottom right
	output.Pos = float4(quadCenter[0].Pos.x + (Dimensions.x / 2), quadCenter[0].Pos.y - (Dimensions.y / 2), oldZ, 1);
	output.Pos = mul(output.Pos, WVP);
	//output.TexCoord = float2(1, 1);
	//output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	triStream.Append(output);
}