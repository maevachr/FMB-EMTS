cbuffer param
{
	float4x4 matWVP; // la matrice de travail
	float2 coordTexHG;
	float2 coordTexBD;
	float vel;
	float target;
	float2 remplissage;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float2 coordTex : TEXCOORD0;
};

VS_Sortie Sprite1VS(float4 Pos : POSITION, float2 coordTex: TEXCOORD)
{
	VS_Sortie sortie = (VS_Sortie)0;
	sortie.Pos = mul(Pos, matWVP);
	// Coordonnées d'application de texture
	sortie.coordTex = coordTexHG + coordTex * (coordTexBD - coordTexHG);
	return sortie;
}

Texture2D textureEntree; // la texture
SamplerState SampleState; // l'état de sampling

float4 Sprite1PS( VS_Sortie vs ) : SV_Target
{
	float4 couleurTexture;
	couleurTexture = textureEntree.Sample(SampleState, vs.coordTex);
	
	if (target > -1.0)
	{
		if (vel < target*0.25)
		{
			couleurTexture.g *= 0;
			couleurTexture.b *= 0;
		
		}
		else if (vel < target*0.75)
		{
			couleurTexture.g *= (vel-target*0.25)/(target*0.50);
			couleurTexture.b *= 0;
		}
		else if (vel < target)
		{
			couleurTexture.b *= 0;
		}
		else
		{
			couleurTexture.r *= 0;
			couleurTexture.b *= 0;
		}
	}
	return couleurTexture;
}
technique11 AfficheSprite
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, Sprite1VS()));
		SetPixelShader(CompileShader(ps_4_0, Sprite1PS()));
		SetGeometryShader(NULL);
	}
}
