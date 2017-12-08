#define NB_LIGHTS 1

cbuffer param
{ 
	struct {
		float4 vLumiere; 		// la position de la source d'éclairage (Point)
		float4 vAEcl; 			// la valeur ambiante de l'éclairage
		float4 vDEcl; 			// la valeur diffuse de l'éclairage 
		float4 vSEcl; 			// la valeur spéculaire de l'éclairage 
	} lights[NB_LIGHTS];
	float4x4 matWorldViewProjLight; // Matrice WVP pour lumière
	float4x4 matWorldViewProj;   // la matrice totale 
	float4x4 matWorld;		// matrice de transformation dans le monde 
	float4 vCamera; 			// la position de la caméra
	float4 vAMat; 			// la valeur ambiante du matériau
	float4 vDMat; 			// la valeur diffuse du matériau 
	float4 vSMat; 			// la valeur spéculaire du matériau 
	float puissance; 		// la puissance de spécularité
	int bTex;		    	// Booléen pour la présence de texture
	int nTex;				// Booléen pour la présence de bump
	float fatt;				//facteur d'attenuation;
}

struct ShadowMapVS_SORTIE
{
	float4 Pos : SV_POSITION;
	float3 Profondeur:	TEXCOORD0;
};

//-------------------------------------------------------------------
// Vertex Shader pour construire le shadow map
//-------------------------------------------------------------------
ShadowMapVS_SORTIE ShadowMapVS( float4 Pos : POSITION )
{
ShadowMapVS_SORTIE Out = (ShadowMapVS_SORTIE)0;

    // Calcul des coordonnées
    Out.Pos = mul( Pos, matWorldViewProjLight);  // WVP de la lumiere

    // Obtenir la profondeur et normaliser avec w
    Out.Profondeur.x = Out.Pos.z ; 
	Out.Profondeur.y = Out.Pos.w ; 
 
    return Out;
}

//-------------------------------------------------------------------
// Pixel Shader pour le shadow map
//-------------------------------------------------------------------
float4 ShadowMapPS( ShadowMapVS_SORTIE entree ) : SV_Target
{
	float4 resultat;
	resultat.x = entree.Profondeur.x/entree.Profondeur.y;
	// retourner la profondeur normalisée [0..1]
	return resultat; 
}


RasterizerState rsCullFront
{
	CullMode = Front;
};

//-------------------------------------------------------------------
// Technique pour le shadow map
//-------------------------------------------------------------------
technique11 ShadowMap 
{
	pass p0 
    {
		VertexShader = compile vs_4_0 ShadowMapVS();
		SetRasterizerState( rsCullFront );
		
 		PixelShader = compile ps_4_0 ShadowMapPS();
		SetGeometryShader(NULL);
	}
}

Texture2D textureEntree;    // la texture
SamplerState SampleState;   // l'état de sampling

Texture2D BumpTexture;    // la texture

Texture2D ShadowTexture0;	// Les textures du shadow map
//Texture2D ShadowTexture1;
SamplerState ShadowMapSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp; 
    AddressV = Clamp;
};

struct VS_Sortie
{
	float4 Pos : SV_Position;
	float3 Norm :    TEXCOORD0;
	float3 PosWorld : TEXCOORD1;
	float3 vDirCam : TEXCOORD2;
	float2 coordTex : TEXCOORD3; 
	float4 PosInMap : TEXCOORD4;
};

//-----------------------------------------------------
// Vertex Shader miniphong avec shadow map
//-----------------------------------------------------
VS_Sortie MiniPhongVS(float4 Pos : POSITION, 
                      float3 Normale : NORMAL, float2 coordTex: TEXCOORD0)  
{
	VS_Sortie sortie = (VS_Sortie)0;
	
	sortie.Pos = mul(Pos, matWorldViewProj); 
	sortie.Norm = mul(Normale, matWorld); 
	
 	sortie.PosWorld = mul(Pos, matWorld);

	sortie.vDirCam = vCamera - sortie.PosWorld; 
	
	sortie.PosInMap = mul(Pos, matWorldViewProjLight);

	// Coordonnées d'application de texture
	sortie.coordTex = coordTex;

	return sortie;
}

#define DIMTEX 2048

float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{
	float4 couleur = (float4) 0;

	for (int i = 0; i < NB_LIGHTS; ++i) {
		
		float3 vDirLum = lights[i].vLumiere - vs.PosWorld; 
		float3 N;
		
		if(nTex > 0) {
			// Échantillonner la couleur du pixel à partir de la texture  
			float3 bump = BumpTexture.Sample(SampleState, vs.coordTex);
			N = normalize(vs.Norm + bump);
		} else {
			N = normalize(vs.Norm);
		}
		
		// Normaliser les paramètres
		float3 L = normalize(vDirLum);
		float3 V = normalize(vs.vDirCam);

		// Valeur de la composante diffuse
		float4 diff = saturate(dot(N, L)); 

		// R = 2 * (N.L) * N - L
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
	
	float4 PosInMap = vs.PosInMap;

	// Calculer la valeur du shadowmapping
	// Calculer les coordonnées de texture (ShadowMap)
	float2 ShadowCoord = 0.5 * PosInMap.xy / PosInMap.w + float2( 0.5, 0.5 );
	ShadowCoord.y = 1.0f - ShadowCoord.y;
	
	float2 PixelActuel = DIMTEX * ShadowCoord; // Pour une texture de 512 X 512

	// Valeur de l'interpolation linéaire           
	float2 lerps = frac( PixelActuel );

	// Lire les valeurs du tableau, avec les vérifications de profondeur
	float Profondeur = PosInMap.z / PosInMap.w ; 
	float3 kernel[9];

	float echelle = 1.0/DIMTEX;

	float2 coord[9];
	coord[0] = ShadowCoord + float2(-echelle,-echelle);
	coord[1] = ShadowCoord + float2(-echelle, 0.0    ); 
	coord[2] = ShadowCoord + float2(-echelle, echelle); 
	coord[3] = ShadowCoord + float2( 0.0    ,-echelle);   
	coord[4] = ShadowCoord + float2( 0.0    , 0.0    );  
	coord[5] = ShadowCoord + float2( 0.0    , echelle);  
	coord[6] = ShadowCoord + float2( echelle,-echelle);  
	coord[7] = ShadowCoord + float2( echelle, 0.0    ); 
	coord[8] = ShadowCoord + float2( echelle, echelle);  

	float shadowStrength = 0.6f;
	// Colonne 1
	kernel[0].x=(ShadowTexture0.Sample(ShadowMapSampler, coord[0])< Profondeur)? shadowStrength : 1.0f;  
	kernel[0].y=(ShadowTexture0.Sample(ShadowMapSampler, coord[1])< Profondeur)? shadowStrength : 1.0f; 
	kernel[0].z=(ShadowTexture0.Sample(ShadowMapSampler, coord[2])< Profondeur)? shadowStrength : 1.0f; 
	// Colonne 2
	kernel[1].x=(ShadowTexture0.Sample(ShadowMapSampler, coord[3])< Profondeur)? shadowStrength : 1.0f;   
	kernel[1].y=(ShadowTexture0.Sample(ShadowMapSampler, coord[4])< Profondeur)? shadowStrength : 1.0f;  
	kernel[1].z=(ShadowTexture0.Sample(ShadowMapSampler, coord[5])< Profondeur)? shadowStrength : 1.0f;  
	// Colonne 3
	kernel[2].x=(ShadowTexture0.Sample(ShadowMapSampler, coord[6])< Profondeur)? shadowStrength : 1.0f;  
	kernel[2].y=(ShadowTexture0.Sample(ShadowMapSampler, coord[7])< Profondeur)? shadowStrength : 1.0f; 
	kernel[2].z=(ShadowTexture0.Sample(ShadowMapSampler, coord[8])< Profondeur)? shadowStrength : 1.0f;  
	
	// Colonne 1
	//kernel[0].x=(ShadowTexture1.Sample(ShadowMapSampler, coord[0])< kernel[0].x)? shadowStrength : kernel[0].x;  
	//kernel[0].y=(ShadowTexture1.Sample(ShadowMapSampler, coord[1])< kernel[0].y)? shadowStrength : kernel[0].y; 
	//kernel[0].z=(ShadowTexture1.Sample(ShadowMapSampler, coord[2])< kernel[0].z)? shadowStrength : kernel[0].z; 
	// Colonne 2
	//kernel[1].x=(ShadowTexture1.Sample(ShadowMapSampler, coord[3])< kernel[1].x)? shadowStrength : kernel[1].x;   
	//kernel[1].y=(ShadowTexture1.Sample(ShadowMapSampler, coord[4])< kernel[1].y)? shadowStrength : kernel[1].y;  
	//kernel[1].z=(ShadowTexture1.Sample(ShadowMapSampler, coord[5])< kernel[1].z)? shadowStrength : kernel[1].z;  
	// Colonne 3
	//kernel[2].x=(ShadowTexture1.Sample(ShadowMapSampler, coord[6])< kernel[2].x)? shadowStrength : kernel[2].x;  
	//kernel[2].y=(ShadowTexture1.Sample(ShadowMapSampler, coord[7])< kernel[2].y)? shadowStrength : kernel[2].y; 
	//kernel[2].z=(ShadowTexture1.Sample(ShadowMapSampler, coord[8])< kernel[2].z)? shadowStrength : kernel[2].z;  
 
	// Les interpolations linéaires
	// Interpoler colonnes 1 et 2
	float3 col12 = lerp( kernel[0], kernel[1], lerps.x );
	// Interpoler colonnes 2 et 3
	float3 col23 = lerp( kernel[1], kernel[2], lerps.x );
	// Interpoler ligne 1 et colonne 1
	float4 lc;
		   lc.x = lerp( col12.x, col12.y, lerps.y );
	// Interpoler ligne 2 et colonne 1
		   lc.y = lerp( col12.y, col12.z, lerps.y );
	// Interpoler ligne 1 et colonne 2
		   lc.z = lerp( col23.x, col23.y, lerps.y );
	// Interpoler ligne 2 et colonne 1
		   lc.w = lerp( col23.y, col23.z, lerps.y );

	// Faire la moyenne
	float ValeurOmbre = (lc.x + lc.y + lc.z + lc.w) / 4.0;
	
	// I = A + D * N.L + (R.V)n
	couleur.r *= ValeurOmbre;
	couleur.g *= ValeurOmbre;
	couleur.b *= ValeurOmbre;
	
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