#pragma once
#include "dispositif.h"
using namespace DirectX;
namespace PM3D
{
 	//  Classe : CObjet3D
	//
	//  BUT : 	Classe de base de tous nos objets 3D
	//
	class CObjet3D 
	{
	public:

   		// Destructeur
		virtual ~CObjet3D(void){}

		virtual void Anime(float tempsEcoule){};
		virtual void Draw()=0;

		virtual XMVECTOR getPosition() const { return XMVECTOR{}; }
		virtual XMVECTOR getDirection() const { return XMVECTOR{}; }

	protected :
		// Constructeur par d�faut
		CObjet3D(void){}
	

	};

}
