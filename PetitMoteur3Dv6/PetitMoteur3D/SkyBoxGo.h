#pragma once

#include "GameObject.h"
#include "RenderComponent.h"
#include "GestionnaireDeTextures.h"

using namespace physx;

namespace PM3D
{
	class SkyBoxGo : public GameObject
	{
	private:
		static constexpr char* typeId = "SkyBoxGo";
	public:

		virtual void OnSpawn(GameObject* _parent) override
		{
			GameObject::OnSpawn(_parent);

			//Set GameObjects

			//Set Components
			//-----RenderComponent
			RenderSkyBoxComponent* r = CreateComponent<RenderSkyBoxComponent>();
			
		}


		virtual void OnUnspawn() override
		{
			GameObject::OnUnspawn();

			//Remove GameObjects
			std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
				delete(go);
			});
			children.clear();

		}

	};
}
