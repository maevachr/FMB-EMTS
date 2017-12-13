// PetitMoteur3D.cpp : définit le point d'entrée pour l'application.

#include <iostream>
#include <string>
#include <sstream>
#include "stdafx.h"
#include "PetitMoteur3D.h"
#include <exception>
#include "Serialiseur.h"
#include "ChargeurAssimp.h"
#include "resource.h"
#include "util.h"
#include <shellapi.h>
#include <algorithm>
#include <WinUser.h>
#include <chrono>
#include <thread>


using namespace PM3D;


struct Arguments {
	int argCount;
	string chemin, fichier, cheminOmb, fichierOmb;
	void init_arguments(LPWSTR *szArgList, int argCount_) {
		argCount = argCount_; 
		if (argCount > 1) {
			wstring path{ (wchar_t*)szArgList[1] };
			string pathstr{ std::begin(path), std::end(path) };
			auto pos = pathstr.rfind("\\");
			chemin = pathstr.substr(0, pos + 1);
			fichier = pathstr.substr(pos + 1, pathstr.size());



			wstring pathOmb{ (wchar_t*)szArgList[2] };
			string pathstrOmb{ std::begin(pathOmb), std::end(pathOmb) };
			auto posOmb = pathstrOmb.rfind("\\");
			cheminOmb = pathstrOmb.substr(0, posOmb + 1);
			fichierOmb = pathstrOmb.substr(posOmb + 1, pathstrOmb.size());

		}
	}
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	// Pour ne pas avoir d'avertissement
	//UNREFERENCED_PARAMETER(hPrevInstance);
	//UNREFERENCED_PARAMETER(lpCmdLine);
	//UNREFERENCED_PARAMETER(nCmdShow);
	
	try
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		LPWSTR *szArgList;
		int argCount;

		szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
		if (szArgList == NULL)
		{
			MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
			return 10;
		}
		Arguments args;
		args.init_arguments(szArgList, argCount);
		
		if (argCount > 1) {

			Serialiseur s;
			CChargeurAssimp chargeur;
			CParametresChargement param;

			param.NomChemin = args.chemin;
			param.NomFichier = args.fichier;
			param.bMainGauche = false;
			param.bInverserCulling = true;

			chargeur.Chargement(param);  // Le chargeur lit le fichier

			string nomOmb = args.cheminOmb + args.fichierOmb;
			std::vector<std::string> nomsTextures = s.EcrireFichierBinaire(chargeur, nomOmb);

			stringstream ss;
			ss << param.NomChemin << param.NomFichier << "\n\na donne le fichier \n\n" << nomOmb << ". " << "\n\nLes textures utilisees sont :\n";
			for (std::string nomTexture : nomsTextures)
			{
				ss << "\n" << nomTexture;
			}
			ss << "\n\nL'operation a reussi.";
			string st = ss.str();
			wstring ws{ st.begin(),st.end() };
			LPCWSTR message = (LPCWSTR)ws.c_str();
			MessageBox(NULL, message, L"Ok", MB_ICONINFORMATION);
		}
		else {

			MessageBox(NULL, L"Argument manquant : fichier obj", L"Erreur", MB_ICONWARNING);
		}


		return (int)1;
	}

	catch (const std::exception& E)
	{
		wchar_t message[35];
		mbtowc(message, E.what(), 34);

		MessageBox(NULL, message, L"Erreur", MB_ICONWARNING);

		return (int)99;
	}

	catch (int codeErreur)
	{
		wchar_t szErrMsg[200];	// Un message d'erreur selon le code

		::LoadString(hInstance, codeErreur, szErrMsg, 200);
		::MessageBox(NULL, szErrMsg, L"Erreur", MB_ICONWARNING);

		return (int)99; // POURQUOI 99???
	}

}
