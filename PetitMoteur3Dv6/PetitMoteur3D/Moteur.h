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
#include "CallBackManager.h"
#include "SpriteManager.h"

namespace PM3D
{

	const int IMAGESPARSECONDE=60;

    //
	//   TEMPLATE�: CMoteur
	//
	//   BUT�: Template servant � construire un objet Moteur qui implantera les
	//         aspects "g�n�riques" du moteur de jeu
	//
	//   COMMENTAIRES�:
	//
	//        Comme plusieurs de nos objets repr�senteront des �l�ments uniques 
	//		  du syst�me (ex: le moteur lui-m�me, le lien vers 
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
				// Propre � la plateforme - (Conditions d'arr�t, interface, messages)
				bBoucle = RunSpecific();	

				// appeler la fonction d'animation
				if (bBoucle) bBoucle = Animation();
			}
		}
		
		virtual int Initialisations()
		{

			// Propre � la plateforme
			InitialisationsSpecific();

			

			// * Initialisation du dispositif de rendu
			pDispositif = CreationDispositifSpecific( CDS_FENETRE );

			//Initialisation de physX
			SimulationManager::GetInstance().InitPhysX();
			
			//Initialisation des lumi�res
			CLightManager::GetInstance().Init();

			//Initialisation des Mesh de rendu
			MeshManager::GetInstance().SetMeshes();
			
			//Initialisation des objets
			SpawnManager::GetInstance().Init();

			MeshManager::GetInstance().InitMeshes(pDispositif);
			SpriteManager::GetInstance().InitSprite(pDispositif);

			//Initialisation du terrain avec la Mesh charg�e
			PhysicManager::GetInstance().InitTerrainPhysic();

			//Ajout des acteurs � la simulation
			PhysicManager::GetInstance().AddActors();

			

			// * Initialisation des matrices de projections
			InitScene();

			// Initialisation des cam�ras
			CCameraManager::GetInstance().Init(&this->matView,
				&this->matProj,
				&this->matViewProj,
				SpawnManager::GetInstance().GetPlayer());

			// * Initialisation des param�tres de l'animation et 
			//   pr�paration de la premi�re image
			InitAnimation();

			return 0;
		}

		virtual bool Animation()
		{
		__int64 TempsCourant;
		float TempsEcoule;

			// m�thode pour lire l'heure et calculer le 
			// temps �coul�
			TempsCourant = GetTimeSpecific();

			// Est-il temps de rendre l'image?
			if (TempsCourant > TempsSuivant)
			{
				// Affichage optimis� 
				pDispositif->Present(); 

				TempsEcoule=static_cast<float>(TempsCourant-TempsPrecedent) 
							* static_cast<float>(EchelleTemps);

				// On pr�pare la prochaine image
				//AnimeScene(TempsEcoule);

				// ON ANIME LA SCENE SI BESOIN
				AnimeScene(TempsEcoule);

				// On rend l'image sur la surface de travail 
   				// (tampon d'arri�re plan)
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

        // Constructeur par d�faut
		CMoteur(void){}
	
        // Destructeur
		~CMoteur(void)
		{
			Cleanup();
		}

		// Sp�cifiques - Doivent �tre implant�s
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

			// premi�re Image
			RenderScene();

			return true;
		}

		// Fonctions de rendu et de pr�sentation de la sc�ne
		virtual bool RenderScene()
		{
			BeginRenderSceneSpecific();

			// Appeler les fonctions de dessin de chaque objet de la sc�ne
		/*	std::vector<CObjet3D*>::iterator It;

			for (It = ListeScene.begin(); It != ListeScene.end(); It++)
			{
					(*It)->Draw();
			}*/

			RenderManager::GetInstance().Draw();
			SpriteManager::GetInstance().Draw();
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

			// D�truire le dispositif
			if (pDispositif) 
			{
				delete pDispositif;
				pDispositif = NULL;
			}
		}

	virtual int InitScene()
	{
		// Initialisation des objets 3D - cr�ation et/ou chargement
		//if (!InitObjets()) return 1;
	
		//// Initialisation des matrices View et Proj
		//// Dans notre cas, ces matrices sont fixes
		//matView = XMMatrixLookAtRH( XMVectorSet( 0.0f, -150.0f, 50.0f, 1.0f ),
  //     								XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
  //                   				XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f ) );

		float champDeVision = XM_PI/4; 	// 45 degr�s
		float ratioDAspect = pDispositif->GetLargeur()/pDispositif->GetHauteur();		
		float planRapproche = 0.05f;
		float planEloigne = 10000000.0f;
		
		matProj = XMMatrixPerspectiveFovRH( 
									champDeVision,
									ratioDAspect,
									planRapproche,
									planEloigne );

		//// Calcul de VP � l'avance
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

		CallBackManager::GetInstance().UpdateTime(tempsEcoule);

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

	