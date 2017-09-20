
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
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;       // Norm color
	float2 uv			: UV;			// UV color
	float4 color		: COLOR;        // RGBA color
};


struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 uvw			: TEXCOORD;
};


VertexToPixel main( VertexShaderInput input )
{
	VertexToPixel output;

	// Copy the view matrix and remove translation
	matrix viewNoMove = view;
	viewNoMove._41 = 0;
	viewNoMove._42 = 0;
	viewNoMove._43 = 0;

	// Calculate output position
	matrix viewProj = mul(viewNoMove, projection);
	output.position = mul(float4(input.position, 1.0f), viewProj);

	// Ensure the vertex is at max depth
	output.position.z = output.position.w;

	// Use the raw vertex position as a direction
	// in space
	output.uvw = input.position;

	return output;
}