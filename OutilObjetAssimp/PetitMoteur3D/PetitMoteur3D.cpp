// PetitMoteur3D.cpp : définit le point d'entrée pour l'application.

#include <iostream>
#include <string>
#include "stdafx.h"
#include "PetitMoteur3D.h"
#include <exception>
#include "Serialiseur.h"
#include "ChargeurAssimp.h"
#include "resource.h"
#include "util.h"


using namespace PM3D;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// Pour ne pas avoir d'avertissement
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

    try
	{
		Serialiseur s;
		CChargeurAssimp chargeur;
		CParametresChargement param;

		param.NomChemin = ".\\modeles\\Arene\\";

		param.NomFichier = "Bordure.obj";		
		param.bMainGauche = false;
		param.bInverserCulling = true;

		chargeur.Chargement(param);  // Le chargeur lit le fichier
		s.EcrireFichierBinaire(chargeur, "areneLimites.omb");
		return (int)1;
    }

	catch (const std::exception& E)
    {
		wchar_t message[35];     
		mbtowc( message, E.what(), 34 );

        MessageBox(NULL, message, L"Erreur", MB_ICONWARNING);
	    
		return (int)99;
    }

	catch (int codeErreur)
  	{
	wchar_t szErrMsg[200];	// Un message d'erreur selon le code

		::LoadString(hInstance, codeErreur, szErrMsg, 200);
		::MessageBox(NULL,szErrMsg,L"Erreur",MB_ICONWARNING);
	  
		return (int)99; // POURQUOI 99???
	}

}




