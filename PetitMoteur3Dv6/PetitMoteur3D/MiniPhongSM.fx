#define NB_LIGHTS 5

cbuffer param
{ 
	struct {
		float4 vLumiere; 		// la position de la source d'éclairage (Point)
		float4 vAEcl; 			// la valeur ambiante de l'éclairage
		float4 vDEcl; 			// la valeur diffuse de l'éclairage 
		float4 vSEcl; 			// la valeur spéculaire de l'éclairage
		float4x4 matWorldViewProjLights; // Matrice WVP pour lumière
		float4 direction;
		float innerAp;
		float outerAp;
		float2 remplissage;
	} lights[NB_LIGHTS];
	float4x4 matWorldViewProjLight; // Matrice WVP pour lumière
	float4x4 matWorldViewProj;   // la matrice totale 
	float4x4 matWorld;		// matrice de transformation dans le monde 
	float4 vCamera; 			// la position de la caméra
	float4 vEMat;           // la valeur émise du matériau du matériau
	float4 vAMat; 			// la valeur ambiante du matériau
	float4 vDMat; 			// la valeur diffuse du matériau 
	float4 vSMat; 			// la valeur spéculaire du matériau 
	float puissance; 		// la puissance de spécularité
	int bTex;		    	// Booléen pour la présence de texture
	int nTex;				// Booléen pour la présence de bump
	float maxLightDist;
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

Texture2D ShadowTextures[NB_LIGHTS];	// Les textures du shadow map

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
	float4 PosInMap[NB_LIGHTS] : TEXCOORD4;
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
	
	for (int i = 0; i < NB_LIGHTS; ++i){
		sortie.PosInMap[i] = mul(Pos, lights[i].matWorldViewProjLights);
	}

	// Coordonnées d'application de texture
	sortie.coordTex = coordTex;

	return sortie;
}

#define DIMTEX 2048

float4 MiniPhongPS( VS_Sortie vs ) : SV_Target
{
	float4 couleur = (float4) 0;
	float4 couleurTexture = (float4) 0;
	float3 diff = (float3) 0;
	float3 S = (float3) 0;
	float3 valeurAmbiente = (float3) 0;
	
	if (bTex>0)
	{
		// Échantillonner la couleur du pixel à partir de la texture  
		couleur = textureEntree.Sample(SampleState, vs.coordTex);
	} else {
		couleur = vAMat;
	}

	for (int i = 0; i < NB_LIGHTS; ++i) {
		float ValeurOmbre = 0.0f;
		float3 vDirLum = vs.PosWorld - lights[i].vLumiere; 
		float ratio = length(vDirLum)/maxLightDist;
		
		//facteur d'attenuation
		float fatt = 1.f - (ratio > 1.f ? 1.f : ratio);
		
		float3 N;
		
		if(nTex > 0) {
			// Échantillonner la couleur du pixel à partir de la texture  
			float3 bump = BumpTexture.Sample(SampleState, vs.coordTex);
			N = normalize(bump);
		} else {
			N = normalize(vs.Norm);
		}
		
		// Normaliser les paramètres
		float3 L = normalize(vDirLum);
		float3 V = normalize(vs.vDirCam);
		
		float T = dot(L, lights[i].direction);
		float intensity;
		if (T > lights[i].innerAp)
			intensity = 1.f;
		else if (T > lights[i].outerAp)
			intensity = 1.f-(lights[i].innerAp-T)/(lights[i].innerAp-lights[i].outerAp);
		else
			intensity = 0.f;

		// Valeur de la composante diffuse
		diff += saturate(dot(N, L)) * lights[i].vDEcl.rgb * vDMat.rgb; 

		// R = 2 * (N.L) * N - L
		float3 R = normalize(2 * diff * N - L);
		
		// Calcul de la spécularité 
		S += pow(saturate(dot(R, V)), puissance) * lights[i].vSEcl.rgb * vSMat.rgb; 
		
		if (intensity > 0.f){

			float4 PosInMap = vs.PosInMap[i];

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

			// Colonne 1
			kernel[0].x=(ShadowTextures[i].Sample(ShadowMapSampler, coord[0])< Profondeur)? 0.0f : 1.0f;  
			kernel[0].y=(ShadowTextures[i].Sample(ShadowMapSampler, coord[1])< Profondeur)? 0.0f : 1.0f; 
			kernel[0].z=(ShadowTextures[i].Sample(ShadowMapSampler, coord[2])< Profondeur)? 0.0f : 1.0f; 
			// Colonne 2
			kernel[1].x=(ShadowTextures[i].Sample(ShadowMapSampler, coord[3])< Profondeur)? 0.0f : 1.0f;   
			kernel[1].y=(ShadowTextures[i].Sample(ShadowMapSampler, coord[4])< Profondeur)? 0.0f : 1.0f;  
			kernel[1].z=(ShadowTextures[i].Sample(ShadowMapSampler, coord[5])< Profondeur)? 0.0f : 1.0f;  
			// Colonne 3
			kernel[2].x=(ShadowTextures[i].Sample(ShadowMapSampler, coord[6])< Profondeur)? 0.0f : 1.0f;  
			kernel[2].y=(ShadowTextures[i].Sample(ShadowMapSampler, coord[7])< Profondeur)? 0.0f : 1.0f; 
			kernel[2].z=(ShadowTextures[i].Sample(ShadowMapSampler, coord[8])< Profondeur)? 0.0f : 1.0f;  
		 
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
			ValeurOmbre = (lc.x + lc.y + lc.z + lc.w) / 4.0;
			
			float3 valeurAmbienteDelta = (1.0f - lights[i].vAEcl.r, 1.0f - lights[i].vAEcl.g, 1.0f - lights[i].vAEcl.b);
			valeurAmbiente += lights[i].vAEcl + valeurAmbienteDelta * ValeurOmbre * fatt * intensity;
		} else {
			valeurAmbiente += lights[i].vAEcl;
		}
	}
	
	valeurAmbiente.rgb /= float(NB_LIGHTS);
	diff.rgb /= float(NB_LIGHTS);
	S.rgb /= float(NB_LIGHTS);
	
	// I = E + A + D * N.L + (R.V)n
	couleur.rgb = vEMat + couleur.rgb * valeurAmbiente.rgb
		+ couleur.rgb * diff.rgb
		+ S.rgb;
	
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