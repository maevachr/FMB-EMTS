#pragma once
#include "Singleton.h"
#include "dispositif.h" 

#include <vector>
#include <fstream>
#include "GestionnaireDeTextures.h"
#include "ChargeurAssimp.h"
#include "CameraManager.h"
#include "LightManager.h"
#include "SimulationManager.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "PhysicManager.h"
#include "SpawnManager.h"
#include "MeshManager.h"

namespace PM3D
{

	const int IMAGESPARSECONDE=60;

    //
	//   TEMPLATE : CMoteur
	//
	//   BUT : Template servant à construire un objet Moteur qui implantera les
	//         aspects "génériques" du moteur de jeu
	//
	//   COMMENTAIRES :
	//
	//        Comme plusieurs de nos objets représenteront des éléments uniques 
	//		  du système (ex: le moteur lui-même, le lien vers 
	//        le dispositif Direct3D), l'utilisation d'un singleton 
	//        nous simplifiera plusieurs aspects.
	//
	template <class T, class TClasseDispositif> class CMoteur :public CSingleton<T>
	{
	public:

		virtual void Run()
		{
		bool bBoucle=true;

			while (bBoucle)
			{
				// Propre à la plateforme - (Conditions d'arrêt, interface, messages)
				bBoucle = RunSpecific();	

				// appeler la fonction d'animation
				if (bBoucle) bBoucle = Animation();
			}
		}
		
		virtual int Initialisations()
		{

			// Propre à la plateforme
			InitialisationsSpecific();

			

			// * Initialisation du dispositif de rendu
			pDispositif = CreationDispositifSpecific( CDS_FENETRE );

			//Initialisation de physX
			SimulationManager::GetInstance().InitPhysX();
			
			//Initialisation des lumières
			CLightManager::GetInstance().Init();

			//Initialisation des Mesh de rendu
			MeshManager::GetInstance().SetMeshes();
			
			//Initialisation des objets
			SpawnManager::GetInstance().Init();

			MeshManager::GetInstance().InitMeshes(pDispositif);

			//Initialisation du terrain avec la Mesh chargée
			PhysicManager::GetInstance().InitTerrainPhysic();

			//Ajout des acteurs à la simulation
			PhysicManager::GetInstance().AddActors();

			

			// * Initialisation des matrices de projections
			InitScene();

			// Initialisation des caméras
			CCameraManager::GetInstance().Init(&this->matView,
				&this->matProj,
				&this->matViewProj,
				SpawnManager::GetInstance().GetPlayer());

			// * Initialisation des paramètres de l'animation et 
			//   préparation de la première image
			InitAnimation();

			return 0;
		}

		virtual bool Animation()
		{
		__int64 TempsCourant;
		float TempsEcoule;

			// méthode pour lire l'heure et calculer le 
			// temps écoulé
			TempsCourant = GetTimeSpecific();

			// Est-il temps de rendre l'image?
			if (TempsCourant > TempsSuivant)
			{
				// Affichage optimisé 
				pDispositif->Present(); 

				TempsEcoule=static_cast<float>(TempsCourant-TempsPrecedent) 
							* static_cast<float>(EchelleTemps);

				// On prépare la prochaine image
				//AnimeScene(TempsEcoule);

				// ON ANIME LA SCENE SI BESOIN
				AnimeScene(TempsEcoule);

				// On rend l'image sur la surface de travail 
   				// (tampon d'arrière plan)
				RenderScene();

				// Calcul du temps du prochain affichage
				TempsPrecedent=TempsCourant;
				TempsSuivant = TempsCourant + EcartTemps;
			}

			return true;
		}

		XMMATRIX GetMatView() { return matView; }
		XMMATRIX GetMatProj() { return matProj; }
		XMMATRIX GetMatViewProj() { return matViewProj; }

    protected :

        // Constructeur par défaut
		CMoteur(void){}
	
        // Destructeur
		~CMoteur(void)
		{
			Cleanup();
		}

		// Spécifiques - Doivent être implantés
		virtual bool RunSpecific()=0;
		virtual int InitialisationsSpecific()=0;
		virtual __int64 GetTimeSpecific()=0;
		virtual TClasseDispositif* CreationDispositifSpecific(const CDS_MODE cdsMode)=0;
		virtual void BeginRenderSceneSpecific()=0;
		virtual void EndRenderSceneSpecific()=0;

		// Autres fonctions
		virtual int InitAnimation()
		{
			TempsSuivant = GetTimeSpecific();
			EchelleTemps = 0.001; 
			EcartTemps = 1000/IMAGESPARSECONDE;
			TempsPrecedent = TempsSuivant; 

			// première Image
			RenderScene();

			return true;
		}

		// Fonctions de rendu et de présentation de la scène
		virtual bool RenderScene()
		{
			BeginRenderSceneSpecific();

			// Appeler les fonctions de dessin de chaque objet de la scène
		/*	std::vector<CObjet3D*>::iterator It;

			for (It = ListeScene.begin(); It != ListeScene.end(); It++)
			{
					(*It)->Draw();
			}*/

			RenderManager::GetInstance().Draw();

			EndRenderSceneSpecific();
			return true;
		}


		virtual void Cleanup()
		{

			// Vider les textures
			CGestionnaireDeTextures::GetInstance().Cleanup();

			SpawnManager::GetInstance().CleanUp();

			//Terminaison de physX
			SimulationManager::GetInstance().TerminatePhysX();

			MeshManager::GetInstance().CleanUp();

			// Détruire le dispositif
			if (pDispositif) 
			{
				delete pDispositif;
				pDispositif = NULL;
			}
		}

	virtual int InitScene()
	{
		// Initialisation des objets 3D - création et/ou chargement
		//if (!InitObjets()) return 1;
	
		//// Initialisation des matrices View et Proj
		//// Dans notre cas, ces matrices sont fixes
		//matView = XMMatrixLookAtRH( XMVectorSet( 0.0f, -150.0f, 50.0f, 1.0f ),
  //     								XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
  //                   				XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f ) );

		float champDeVision = XM_PI/4; 	// 45 degrés
		float ratioDAspect = pDispositif->GetLargeur()/pDispositif->GetHauteur();		
		float planRapproche = 0.05f;
		float planEloigne = 10000000.0f;
		
		matProj = XMMatrixPerspectiveFovRH( 
									champDeVision,
									ratioDAspect,
									planRapproche,
									planEloigne );

		//// Calcul de VP à l'avance
		//matViewProj = matView * matProj;

		return 0;
	}

	bool AnimeScene(float tempsEcoule)
	{
		PhysicManager::GetInstance().AddActors();
		// Prendre en note le statut du clavier
		InputManager::GetInstance().ProcessInput();

		//Mise a jour de la simulation physique
		SimulationManager::GetInstance().Update();
		PhysicManager::GetInstance().UpdateGoTransform();

		//ANIME SCENE

		CCameraManager::GetInstance().AnimeScene(tempsEcoule);
		CLightManager::GetInstance().AnimeScene(tempsEcoule);

		SpawnManager::GetInstance().Update();

		return true;
	}


	protected:
		// Variables pour le temps de l'animation
		__int64 TempsSuivant;
		__int64 TempsPrecedent;
		unsigned long EcartTemps;
		double EchelleTemps;

		// Le dispositif de rendu
		TClasseDispositif* pDispositif;

		// Le seul gestionnaire de saisie
		CDIManipulateur GestionnaireDeSaisie;

		// Les matrices
		XMMATRIX matView;
		XMMATRIX matProj;
		XMMATRIX matViewProj;

    };


} // namespace PM3D

	