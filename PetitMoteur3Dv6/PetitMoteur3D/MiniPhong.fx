#define NB_LIGHTS 2

cbuffer param
{ 
	struct {
		float4 vLumiere; 		// la position de la source d'éclairage (Point)
		float4 vAEcl; 			// la valeur ambiante de l'éclairage
		float4 vDEcl; 			// la valeur diffuse de l'éclairage 
		float4 vSEcl; 			// la valeur spéculaire de l'éclairage 
	} lights[NB_LIGHTS];
	float4x4 matWorldViewProj;   // la matrice totale 
	float4x4 matWorld;		// matrice de transformation dans le monde 
	float4 vCamera; 			// la position de la caméra
	float4 vAMat; 			// la valeur ambiante du matériau
	float4 vDMat; 			// la valeur diffuse du matériau 
	float4 vSMat; 			// la valeur spéculaire du matériau 
	float puissance; 		// la puissance de spécularité
	int bTex;		    		// Booléen pour la présence de texture
	float2 remplissage;
}

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm :    TEXCOORD0;
	float3 PosWorld : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3; 
};

VS_Sortie MiniPhongVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex: TEXCOORD)  
{
VS_Sortie sortie = (VS_Sortie)0;
	
	sortie.Pos = mul(Pos, matWorldViewProj); 
	sortie.Norm = mul(Normale, matWorld); 
	
 	sortie.PosWorld = mul(Pos, matWorld);

	sortie.vDirCam = vCamera - sortie.PosWorld; 

    // Coordonnées d'application de texture
    sortie.coordTex = coordTex;

	return sortie;
}

Texture2D textureEntree;  // la texture
SamplerState SampleState;  // l'état de sampling

float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{
float4 couleur = (float4)0; 
	for(int i = 0; i < NB_LIGHTS; i++){
	float3 vDirLum = lights[i].vLumiere - vs.PosWorld; 

	// Normaliser les paramètres
	float3 N = normalize(vs.Norm);
 	float3 L = normalize(vDirLum);
	float3 V = normalize(vs.vDirCam);

	// Valeur de la composante diffuse
	float4 diff = saturate(dot(N, L)); 

	// R = 2 * (N.L) * N – L
	float3 R = normalize(2 * diff * N - L);
    
	// Calcul de la spécularité
	float4 S = pow(saturate(dot(R, V)), puissance);

	float4 couleurTexture;  

	if (bTex>0)
	{
		// Échantillonner la couleur du pixel à partir de la texture  
		couleurTexture = textureEntree.Sample(SampleState, vs.coordTex);

		// I = A + D * N.L + (R.V)n
		couleur = couleur + couleurTexture * lights[i].vAEcl * vAMat +
			couleurTexture * lights[i].vDEcl * vDMat * diff +
			lights[i].vSEcl * vSMat * S;
	}
	else
	{
		couleur = couleur + lights[i].vAEcl * vAMat + lights[i].vDEcl * vDMat * diff +
			lights[i].vSEcl * vSMat * S;
	}
}
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
