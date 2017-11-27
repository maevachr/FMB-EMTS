#pragma once
#include "InputComponent.h"


namespace PM3D
{
	class MonsterTruckInputComponent : public InputComponent
	{
	public:
		static constexpr char* typeId = "MonsterTruckInputComponent";
	public:
		virtual void ProcessInput(CDIManipulateur* pGestionnaireDeSaisie);
	};

}