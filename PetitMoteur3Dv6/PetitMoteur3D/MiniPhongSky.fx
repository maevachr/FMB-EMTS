
cbuffer param
{ 
	float4x4 matWorldViewProj;   // la matrice totale 
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float2 coordTex : TEXCOORD0; 
};

VS_Sortie MiniPhongVS(float4 Pos : POSITION, float2 coordTex: TEXCOORD)  
{
VS_Sortie sortie = (VS_Sortie)0;
	
	sortie.Pos = mul(Pos, matWorldViewProj); 
	
    // Coordonnées d'application de texture
    sortie.coordTex = coordTex;

	return sortie;
}

Texture2D textureEntree;  // la texture
SamplerState SampleState;  // l'état de sampling

float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{
	float4 couleur = (float4)0; 
 	couleur = textureEntree.Sample(SampleState, vs.coordTex);
	return couleur;
}

technique11 MiniPhong
{
    pass pass0
    {
        SetVertexShader(CompileShader(vs_4_0, MiniPhongVS()));
        SetPixelShader(CompileShader(ps_4_0, MiniPhongPS()));
        SetGeometryShader(NULL);
    }
}
