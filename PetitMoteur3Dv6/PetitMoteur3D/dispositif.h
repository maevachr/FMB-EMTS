#pragma once

namespace PM3D
{
	// Constantes pour mode fen�tr� ou plein �cran
	enum CDS_MODE
	{
		 CDS_FENETRE,
		 CDS_PLEIN_ECRAN
	};

  	//
	//  Classe : CDispositif
	//
	//  BUT : 	Classe servant � construire un objet Dispositif 
	//				qui implantera les aspects "g�n�riques" du dispositif de 
	//				rendu
	//
	class CDispositif 
	{
	public:
		virtual void Present()
		{
			PresentSpecific();
		};

		virtual void PresentSpecific()=0;

		float GetLargeur() {return largeurEcran;}
		float GetHauteur() {return hauteurEcran;}

  	protected :
   // Constructeur par d�faut
		CDispositif(void){}
	
   	// Destructeur
		virtual ~CDispositif(void){}

		float largeurEcran;
		float hauteurEcran;


	};

}
