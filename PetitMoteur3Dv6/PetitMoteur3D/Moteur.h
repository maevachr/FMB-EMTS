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
#include "BillBoardManager.h"
#include "BillBoardComponentManager.h"
#include "StateStack.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "EndState.h"

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
	template <class T, class TClasseDispositif> 
	class CMoteur : public CSingleton<T>
	{
	public:

		static CMoteur<T, TClasseDispositif>& GetInstance() {
			return CSingleton<T>::GetInstance();
		}

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

			//Initialisation des Mesh de rendu
			MeshManager::GetInstance().SetMeshes();
			BillBoardManager::GetInstance().InitBillBoard(pDispositif);

			//Initialisation des objets
			SpawnManager::GetInstance().Init();

			//Initialisation des lumi�res
			CLightManager::GetInstance().Init();

			//Initialisation des textures pour les ombres
			CLightManager::GetInstance().InitShadows(pDispositif);

			//Mesh-Sprite-BillBoard
			MeshManager::GetInstance().InitMeshes(pDispositif);
			SpriteManager::GetInstance().Init(pDispositif);
			

			//Initialisation du terrain avec la Mesh charg�e
			PhysicManager::GetInstance().InitStaticComponents();

			//Ajout des acteurs � la simulation
			PhysicManager::GetInstance().AddActors();

			// Initialisation des cam�ras
			CCameraManager::GetInstance().Init(pDispositif, 
				&this->matView,
				&this->matProj,
				&this->matViewProj,
				SpawnManager::GetInstance().GetPlayer());

			mStateStack = new StateStack(State::Context(pDispositif));
			registerStates();
			mStateStack->pushState(States::Menu);
			mStateStack->applyPendingChanges();


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

				// Update
				AnimeScene(TempsEcoule);

				// Draw
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
						
			mStateStack->draw();
			
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

		void ProcessInput() {
			mStateStack->ProcessInput();
		}

		bool AnimeScene(float tempsEcoule)
		{
			ProcessInput();
			mStateStack->update(tempsEcoule);
			return true;
		}



	void registerStates()
	{
		mStateStack->registerState<MenuState>(States::Menu);
		mStateStack->registerState<GameState>(States::Game);
		mStateStack->registerState<PauseState>(States::Pause);
		mStateStack->registerState<EndState>(States::End);
	}

	public:
		void drawGame() {
			// pour le post effect, rendu sur une texture
			SpriteManager::GetInstance().GetPost()->DebutPostEffect();
			BeginRenderSceneSpecific();

			CLightManager::GetInstance().ResetShadowTextures(pDispositif);
			RenderManager::GetInstance().Draw();
			BillBoardComponentManager::GetInstance().Draw();

			EndRenderSceneSpecific();
			SpriteManager::GetInstance().GetPost()->FinPostEffect();
			// post effect fini
			//if (mStateStack->size() == 2)
				SpriteManager::GetInstance().Draw();
		}

	protected:
		// Variables pour le temps de l'animation
		__int64 TempsSuivant;
		__int64 TempsPrecedent;
		unsigned long EcartTemps;
		double EchelleTemps;

		// Le dispositif de rendu
		TClasseDispositif* pDispositif;

		//Menu
		StateStack* mStateStack;

		// Les matrices
		XMMATRIX matView;
		XMMATRIX matProj;
		XMMATRIX matViewProj;

    };


} // namespace PM3D

	