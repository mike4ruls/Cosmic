
cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};


struct VertexShaderInput
{ 
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float4 color		: COLOR;
	float size : SIZE;
};


struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
	float4 color		: TEXCOORD1;
};


VertexToPixel main( VertexShaderInput input )
{
	VertexToPixel output;

	// Calculate output position
	matrix viewProj = mul(view, projection);
	output.position = mul(float4(input.position, 1.0f), viewProj);

	// Use UV to offset position (billboarding)
	float2 offset = input.uv * 2 - 1;
	offset *= input.size;
	offset.y *= -1;
	output.position.xy += offset;

	// Pass uv through
	output.uv = input.uv;
	output.color = input.color;

	return output;
}