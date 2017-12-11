#include "stdafx.h"
#include "InputManager.h"
#include "util.h"
#include "resource.h"
#include "InputComponent.h"

namespace PM3D
{
	void InputManager::Init(HINSTANCE hInstance, HWND hWnd)
	{
		pGestionnaireDeSaisie.Init(hInstance, hWnd);
	}
}