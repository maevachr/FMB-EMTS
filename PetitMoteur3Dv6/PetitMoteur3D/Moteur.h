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
#include "TextureRenderer.h"
#include "SoundManager.h"

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

			SoundManager::GetInstance().Init();

			//Initialisation de physX
			SimulationManager::GetInstance().InitPhysX();

			//Initialisation des Mesh de rendu
			MeshManager::GetInstance().SetMeshes();
			BillBoardManager::GetInstance().InitBillBoard(pDispositif);

			//Initialisation des objets
			SpawnManager::GetInstance().Init();

			//Initialisation des lumières
			CLightManager::GetInstance().Init();

			//Initialisation des textures pour les ombres
			CLightManager::GetInstance().InitShadows(pDispositif);

			//Mesh-Sprite-BillBoard
			MeshManager::GetInstance().InitMeshes(pDispositif);
			SpriteManager::GetInstance().Init(pDispositif);
			
			//Initialisation du terrain avec la Mesh chargée
			PhysicManager::GetInstance().InitStaticComponents();

			//Ajout des acteurs à la simulation
			PhysicManager::GetInstance().AddActors();

			// Initialisation des caméras
			CCameraManager::GetInstance().Init(pDispositif, 
				&this->matView,
				&this->matProj,
				&this->matViewProj,
				SpawnManager::GetInstance().GetPlayer());

			tvRenderer.Initialize(pDispositif->GetD3DDevice(), 1024, 768);
			bloom.Initialize(pDispositif->GetD3DDevice(), 1024, 768);
			blur.Initialize(pDispositif->GetD3DDevice(), 1024, 768);
			combined.Initialize(pDispositif->GetD3DDevice(), 1024, 768);
			BillBoardManager::GetInstance().GetBillBoard("tv")->SetResourceView(tvRenderer.GetShaderResourceView());

			mStateStack = new StateStack(State::Context(pDispositif));
			registerStates();
			mStateStack->pushState(States::Menu);
			mStateStack->applyPendingChanges();


			// * Initialisation des paramètres de l'animation et 
			//   préparation de la première image
			InitAnimation();

			return 0;
		}

	private:
		float TempsEcoule;
	public:
		float GetTempsEcoule() { return TempsEcoule; }


		virtual bool Animation()
		{
			__int64 TempsCourant;
			

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

			// Détruire le dispositif
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
			
			SpriteManager::GetInstance().GetPost()->FinPostEffect();
			EndRenderSceneSpecific();

			auto oldRenderTarget = pDispositif->GetRenderTargetView();
			auto oldDepthStencil = pDispositif->GetDepthStencilView();
			bloom.SetRenderTarget(pDispositif->GetImmediateContext());
			bloom.ClearRenderTarget(pDispositif->GetImmediateContext(), 0.0f, 0.0f, 1.0f, 1.0f);
			SpriteManager::GetInstance().GetPost()->ExtractBloom();
			pDispositif->GetImmediateContext()->OMSetRenderTargets(1, &oldRenderTarget, oldDepthStencil);

			blur.SetRenderTarget(pDispositif->GetImmediateContext());
			blur.ClearRenderTarget(pDispositif->GetImmediateContext(), 0.0f, 0.0f, 1.0f, 1.0f);
			SpriteManager::GetInstance().GetPost()->DrawBlur(bloom.GetShaderResourceView());
			pDispositif->GetImmediateContext()->OMSetRenderTargets(1, &oldRenderTarget, oldDepthStencil);

			combined.SetRenderTarget(pDispositif->GetImmediateContext());
			combined.ClearRenderTarget(pDispositif->GetImmediateContext(), 0.0f, 0.0f, 1.0f, 1.0f);
			SpriteManager::GetInstance().GetPost()->CombineSceneAndBloom(blur.GetShaderResourceView());
			pDispositif->GetImmediateContext()->OMSetRenderTargets(1, &oldRenderTarget, oldDepthStencil);

			SpriteManager::GetInstance().GetPost()->Draw(combined.GetShaderResourceView());
			SpriteManager::GetInstance().DrawSprites();

			tvRenderer.SetRenderTarget(pDispositif->GetImmediateContext());
			tvRenderer.ClearRenderTarget(pDispositif->GetImmediateContext(), 0.0f, 0.0f, 1.0f, 1.0f);
			SpriteManager::GetInstance().GetPost()->Draw();
			pDispositif->GetImmediateContext()->OMSetRenderTargets(1, &oldRenderTarget, oldDepthStencil);
				
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

		TextureRenderer bloom;
		TextureRenderer blur;
		TextureRenderer combined;
		TextureRenderer tvRenderer;

    };
		

} // namespace PM3D

	