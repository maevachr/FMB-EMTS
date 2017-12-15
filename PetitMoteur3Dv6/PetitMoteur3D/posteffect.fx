struct VS_Sortie
{
	float4 Pos : SV_POSITION;
	float2 CoordTex : TEXCOORD0;
};

Texture2D textureEntree;    // la texture
SamplerState SampleState;   // l'?tat de sampling

float distance;

//-------------------------------------------------------
// Vertex Shader ?Nul?
//-------------------------------------------------------
VS_Sortie NulVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD )
{
VS_Sortie sortie = (VS_Sortie)0;
   
  sortie.Pos = Pos; 
  sortie.CoordTex = CoordTex;

  return sortie;
}

//-----------------------------------------------------
// Pixel Shader ?Nul?
//-----------------------------------------------------
float4 NulPS( VS_Sortie vs) : SV_Target
{
float4 couleur; 

    couleur = textureEntree.Sample(SampleState, vs.CoordTex);   
     
    return couleur;
}

float4 RadialBlurPS(VS_Sortie vs) : SV_Target 
{
float4 couleur;
float4 ct; 
float2 tc = vs.CoordTex;
float d, dx, dy;

 	couleur = 0;
	float x = tc.x*2 - 1.0;
	float y = tc.y*2 - 1.0;
	dx = sqrt(x*x);  // Distance du centre
	dy = sqrt(y*y);  // Distance du centre

	dx = x * (distance*dx);  	// Le d?grad? (blur) est en fonction de la
	dy = y * (distance*dy);	 	// distance du centre et de la variable distance.

	x = x - (dx*10);   // Vous pouvez jouer avec le nombre d'it?rations
	y = y - (dy*10);

	tc.x = (x+1.0)/2.0;
	tc.y = (y+1.0)/2.0;	
	
	for (int i = 0; i<10; i++)   // Vous pouvez jouer avec le nombre d'it?rations
	{
	   ct = textureEntree.Sample(SampleState, tc); 
	   couleur = couleur * 0.6 + ct * 0.4; // Vous pouvez ?jouer? avec les %

	   x = x + dx;
	   y = y + dy;

	   tc.x = (x+1.0)/2.0;
	   tc.y = (y+1.0)/2.0;	
	}

	return couleur;
}

// Pixel shader extracts the brighter areas of an image.
// This is the first step in applying a bloom postprocess.

float BloomThreshold = 0.25f;

float4 BloomExtractPS(VS_Sortie vs) : SV_Target
{
    // Look up the original image color.
    float4 c = textureEntree.Sample(SampleState, vs.CoordTex);

    // Adjust it to keep only values brighter than the specified threshold.
    return saturate((c - BloomThreshold) / (1 - BloomThreshold));
}

float BloomIntensity = 1.2f;
float BaseIntensity = 1.5f;

float BloomSaturation = 1.2f;
float BaseSaturation = 1.2f;
Texture2D bloomTexture;

// Helper for modifying the saturation of a color.
float4 AdjustSaturation(float4 color, float saturation)
{
    // The constants 0.3, 0.59, and 0.11 are chosen because the
    // human eye is more sensitive to green light, and less to blue.
    float grey = dot(color, float3(0.3, 0.59, 0.11));

    return lerp(grey, color, saturation);
}

float4 BloomCombinePS(VS_Sortie vs) : SV_Target
{
    // Look up the bloom and original base image colors.
    float4 bloom = bloomTexture.Sample(SampleState, vs.CoordTex);
    float4 base = textureEntree.Sample(SampleState, vs.CoordTex);

    // Adjust color saturation and intensity.
    bloom = AdjustSaturation(bloom, BloomSaturation) * BloomIntensity;
    base = AdjustSaturation(base, BaseSaturation) * BaseIntensity;

    // Darken down the base image in areas where there is a lot of bloom,
    // to prevent things looking excessively burned-out.
    base *= (1 - saturate(bloom));

    // Combine the two images.
    return base + bloom;
}

#define SAMPLE_COUNT 7
 
float offsetX;
float offsetY;
 
float4 GaussianBlurPS(VS_Sortie vs) : SV_Target
{
	float4 c = 0;
    float2 ct = vs.CoordTex ;
	float2 ct2;
	float weight = 0;
	float weightTotal = 0;
    
    // Combine a number of weighted image filter taps.
    for (int y = -SAMPLE_COUNT/2; y < SAMPLE_COUNT-SAMPLE_COUNT/2; y++)
    {
		for (int x = -SAMPLE_COUNT/2; x < SAMPLE_COUNT-SAMPLE_COUNT/2; x++)
		{
			ct2 = ct;
			ct2.x += x * offsetX;
			ct2.y += y * offsetY;
			weight = float(1.f/(1.f + abs(x) + abs(y)));
			weightTotal += weight;
			c = c + (textureEntree.Sample(SampleState, ct2) * weight);
		}
    }
	
    c /= weightTotal;
	//c /= SAMPLE_COUNT;
 
    return c;
}

technique11 Nul 
{
	pass p0 
	{
		VertexShader = compile vs_4_0 NulVS();
		PixelShader = compile ps_4_0 NulPS();
		SetGeometryShader(NULL);
	}
}


technique11 RadialBlur 
{
	pass p0 
	{
		VertexShader = compile vs_4_0 NulVS();
		PixelShader = compile ps_4_0 RadialBlurPS();
		SetGeometryShader(NULL);
	}
}

technique11 BloomExtract
{
    pass p0
    {
		VertexShader = compile vs_4_0 NulVS();
        PixelShader = compile ps_4_0 BloomExtractPS();
		SetGeometryShader(NULL);
    }
}

technique11 BloomCombine
{
    pass p0
    {
		VertexShader = compile vs_4_0 NulVS();
        PixelShader = compile ps_4_0 BloomCombinePS();
		SetGeometryShader(NULL);
    }
}

technique11 GaussianBlur
{
    pass p0
    {
	    VertexShader = compile vs_4_0 NulVS();
        PixelShader = compile ps_4_0 GaussianBlurPS();
		SetGeometryShader(NULL);
    }
}

