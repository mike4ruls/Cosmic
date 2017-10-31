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
	float uvXOffset;
	float uvYOffset;
	float reflectance;
};
Texture2D surfaceTexture : register(t0);
TextureCube skyTexture : register(t1);
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
	float2 newUV = float2(input.uv.x * uvXOffset, input.uv.y * uvYOffset);
	float4 color = surfaceTexture.Sample(basicSampler, newUV) * input.color;
	input.normal = normalize(input.normal);
	float4 allLights = float4(0.0f, 0.0f, 0.0f, 1.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//float4 ambient = dirLights.amb;

	float3 E = input.camPos - input.worldPos;
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

	float radius = 5;
	float minLight = 0.08;
	float a = 0.1;

	// Fall off radius
	//float b = 0.01;
	float b = 1.0 / (radius * radius * minLight);

	for (int j = 0; j < pCount; j++)
	{
		float4 ambient = pointLights[j].amb;
		float dist = distance(input.worldPos, pointLights[j].lightPos);

		//Sharp fade in the distance attenuation
		//float att = clamp(1.0 - dist*dist/(radius*radius), 0.0, 1.0);

		//Nice fade in the distance attenuation
		float att = 1.0 / (1.0 + a*dist + b*dist*dist);

		//float3 E = input.camPos - input.worldPos;
		float3 L = normalize(pointLights[j].lightPos - input.worldPos);
		//float3 H = normalize(L + E);

		float halfWay = clamp(dot(input.normal, -L), 0 , 1);

		allLights += att * (ambient + (pointLights[j].lightColor *  halfWay));
	}
	
	color *= allLights;
	//color = float4(0.8f, 0.8f, 0.8f, 1.0f);
	//color = float4(dirLights[0].lightColor.xyz, 1.0);

	float3 skyRefl = reflect(-E, input.normal);
	float4 reflColor = skyTexture.Sample(basicSampler, skyRefl);
	return lerp(color, reflColor, reflectance);

	//return color;
}