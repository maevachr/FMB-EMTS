cbuffer param
{
	float4x4 matWVP; // la matrice de travail
	float2 coordTexHG;
	float2 coordTexBD;
	float masquageY;
	float3 remplissage;
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
	if(masquageY >= vs.coordTex.y){
		if(couleurTexture.r < 1.0f || couleurTexture.g < 1.0f || couleurTexture.b < 1.0f){
			couleurTexture.a = 0.2f;
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
