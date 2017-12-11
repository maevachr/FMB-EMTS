#pragma once

#include "moteur.h"
#include "dispositifD3D11.h" 

namespace PM3D
{
#define MAX_LOADSTRING 100

class CMoteurWindows : public CMoteur<CMoteurWindows, CDispositifD3D11>
{
public:
	
	CMoteurWindows(void);
	~CMoteurWindows(void);

public:
	void SetWindowsAppInstance(HINSTANCE hInstance);
	static CMoteurWindows& GetInstance() {
		return CSingleton<CMoteurWindows>::GetInstance();
	}

protected:
	ATOM	MyRegisterClass(HINSTANCE hInstance);
	bool InitAppInstance();
	int Show(void);

// Les fonctions sp�cifiques
protected:
	virtual int InitialisationsSpecific();
	virtual bool RunSpecific();
public:
	__int64 CMoteurWindows::GetTimeSpecific();
protected:
	virtual CDispositifD3D11* CreationDispositifSpecific(const CDS_MODE cdsMode);
	virtual void BeginRenderSceneSpecific();
	virtual void EndRenderSceneSpecific();


protected:
	// Fonctions "Callback" -- Doivent �tre statiques
	static LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	static INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

protected:
	HACCEL hAccelTable;						// handle Windows de la table des acc�l�rateurs
	static HINSTANCE hAppInstance;			// handle Windows de l'instance actuelle de l'application
	HWND hMainWnd;							// handle Windows de la fen�tre principale
	TCHAR szWindowClass[MAX_LOADSTRING];	// le nom de la classe de fen�tre principale
};

}
