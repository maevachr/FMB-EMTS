#pragma once
#include <vector>
#include <assert.h>
#include <algorithm>
#include "DIManipulateur.h"

namespace PM3D
{
	class InputComponent;

	class InputManager
	{
	private:
		InputManager() = default;
		~InputManager() = default;
	public:
		InputManager(const InputManager&) = delete;
		InputManager operator=(const InputManager&) = delete;

	public:
		static InputManager& GetInstance()
		{
			static InputManager im;
			return im;
		}

	private:
		std::vector<InputComponent*> iComponents;

	public:
		void CreateComponent(InputComponent* ic)
		{
			iComponents.push_back(ic);
		}
		void RemoveComponent(InputComponent* ic)
		{
			auto it = std::find(begin(iComponents), end(iComponents), ic);
			assert(it != iComponents.end());
			iComponents.erase(it);
		}
	private:
		CDIManipulateur pGestionnaireDeSaisie;
	public:
		void Init(HINSTANCE hInstance, HWND hWnd);
		void ProcessInput();
	};
}