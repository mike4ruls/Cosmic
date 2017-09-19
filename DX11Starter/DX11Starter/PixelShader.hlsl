struct DirectionalLight {
	float4 lightColor;
	float4 amb;
	float3 dirLight;
	float1 inten;
};
struct PointLight {
	float4 lightColor;
	float4 amb;
	float3 lightPos;
	float1 inten;
};
struct SpotLight {
	float4 lightColor;
	float4 amb;
	float3 lightPos;
	float1 inten;
};

cbuffer externalData : register(b0)
{
	DirectionalLight dirLights[10];
	DirectionalLight pointLights[10];
	DirectionalLight spotLights[10];
	int dCount;
	int pCount;
	int sCount;
}
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
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;       // Norm color
	float2 uv			: UV;			// UV color
	float4 color		: COLOR0;
	float3 camPos		: COLOR1;
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
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering

	//float4 color = float4(0.8f, 0.8f, 0.8f, 1.0f);
	float4 color = input.color;
	float4 allLights = float4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < dCount; i++)
	{
		float4 ambient = dirLights[i].amb;

		//float3 E = input.position.xyz - input.camPos;
		float3 L = dirLights[i].dirLight;
		//float3 H = normalize(-L + E);

		float halfWay = saturate(dot(input.normal, -L));
		float specAmt = pow(halfWay, 4.0f);

		allLights += ambient + (dirLights[i].lightColor * dot(input.normal, halfWay)) + specAmt;
	}
	
	color *= allLights;
	//color = float4(input.normal, 1.0);

	return color;
}