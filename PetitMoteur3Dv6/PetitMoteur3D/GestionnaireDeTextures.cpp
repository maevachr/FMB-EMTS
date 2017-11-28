
#include "StdAfx.h"
#include "dispositifD3D11.h"
#include "GestionnaireDeTextures.h"

namespace PM3D
{

CTexture* const CGestionnaireDeTextures::GetNewTexture(const wchar_t* filename_in,CDispositifD3D11* pDispositif)
{	
	CTexture* pTexture;

	// On v�rifie si la texture est d�j� dans notre liste
	pTexture = GetTexture(filename_in);

	// Si non, on la cr�e
	if (!pTexture)
	{
		pTexture = new CTexture(filename_in, pDispositif);

		// Puis, il est ajout� � la sc�ne
		ListeTextures.push_back(pTexture);
	}

	return pTexture;
}

CTexture* const CGestionnaireDeTextures::GetTexture(const wchar_t* filename_in)
{
CTexture* pTexture = 0;
std::vector<CTexture*>::iterator It;
	
	It = ListeTextures.begin();

	while ( pTexture==0 && It != ListeTextures.end() )
	{
		if ( wcscmp( (*It)->GetFilename(), filename_in)==0)
		{
			pTexture = *It;
		}

		It++;
	}

	return pTexture;
}

void CGestionnaireDeTextures::Cleanup()
{
// d�truire les objets
std::vector<CTexture*>::iterator It;

	for (It = ListeTextures.begin(); It != ListeTextures.end();It++)
	{
		delete (*It);
	}

	ListeTextures.clear();
}

} 

