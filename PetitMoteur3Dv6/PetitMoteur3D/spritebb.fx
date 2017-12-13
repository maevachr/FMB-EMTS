cbuffer param
{
	float4x4 matWVP; // la matrice de travail
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
	sortie.coordTex = coordTex;
	return sortie;
}

Texture2D textureEntree; // la texture
SamplerState SampleState; // l'état de sampling

float4 Sprite1PS( VS_Sortie vs ) : SV_Target
{
	float4 couleurTexture;
	couleurTexture = textureEntree.Sample(SampleState, vs.coordTex);
	
	if (target > 0.0)
	{
		if (vel < target/2.0)
		{
			couleurTexture.g *= 0;
			couleurTexture.b *= 0;
		
		}
		else if (vel < target)
		{
			couleurTexture.r *= 0;
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
