cbuffer externalData : register(b0)
{
	float uvXOffset;
	float uvYOffset;
};
Texture2D surfaceTexture : register(t0);
SamplerState basicSampler : register(s0);
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;     // Norm color
	float2 uv			: UV;			// UV color
	float4 color		: COLOR0;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float2 newUV = float2(input.uv.x * uvXOffset, input.uv.y * uvYOffset);
	float4 color = surfaceTexture.Sample(basicSampler, newUV) * input.color;

	return color;
}