
// External texture-related data
TextureCube Sky			: register(t1);
SamplerState Sampler	: register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return Sky.Sample(Sampler, input.uvw);
}