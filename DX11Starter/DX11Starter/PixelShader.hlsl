struct LightComponent {
	float4 lightColor;
	float4 amb;
	float3 lightPos;
	float1 inten;
	float3 dirLight;
	float1 pad;
};

cbuffer externalData : register(b0)
{
	LightComponent dirLights[10];
	LightComponent pointLights[10];
	LightComponent spotLights[10];
	int dCount;
	int pCount;
	int sCount;
};
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
	float3 worldPos		: POSITION;
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
	input.normal = normalize(input.normal);
	float4 allLights = float4(0.0f, 0.0f, 0.0f, 1.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//float4 ambient = dirLights.amb;

	////float3 E = input.position.xyz - input.camPos;
	//float3 L = normalize(dirLights.dirLight);
	////float3 H = normalize(-L + E);

	//float halfWay = saturate(dot(input.normal, -L));
	//float specAmt = pow(halfWay, 1.5f);

	//allLights += ambient + (dirLights.lightColor * dot(input.normal, halfWay)) + specAmt;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////


	for(int i = 0; i < dCount; i++)
	{
		//float dirLightAmmount = saturate(dot(input.normal, -dirLights[i].dirLight));

		float4 ambient = dirLights[i].amb;

		//float3 E = input.position.xyz - input.camPos;
		float3 L = normalize(dirLights[i].dirLight);
		//float3 H = normalize(-L + E);

		float halfWay = saturate(dot(input.normal, -L));
		float specAmt = pow(halfWay, 1.5f);

		//allLights += ambient + (dirLights[i].lightColor * dot(input.normal, halfWay)) + specAmt;
		allLights += ambient + (dirLights[i].lightColor * halfWay) + specAmt;
	}

	for (int j = 0; j < pCount; j++)
	{
		float4 ambient = pointLights[j].amb;

		float3 E = input.camPos - input.worldPos;
		float3 L = normalize(pointLights[j].lightPos - input.worldPos);
		float3 H = normalize(L + E);

		float halfWay = saturate(dot(input.normal, H));
		float specAmt = pow(halfWay, 1.0f);

		allLights += ambient + (pointLights[j].lightColor *  dot(input.normal, -L) + specAmt);
	}
	
	color *= allLights;
	//color = float4(dirLights[0].lightColor.xyz, 1.0);

	return color;
}