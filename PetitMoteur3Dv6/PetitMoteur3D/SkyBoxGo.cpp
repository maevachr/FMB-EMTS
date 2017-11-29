#include "stdafx.h"
#include "SkyBoxGo.h"
#include "RenderComponent.h"

namespace PM3D
{
	void PM3D::SkyBoxGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);

		//Set GameObjects

		//Set Components
		//-----RenderComponent
		RenderSkyBoxComponent* r = CreateComponent<RenderSkyBoxComponent>();
		r->GetSkyBoxMesh();
	}

	void PM3D::SkyBoxGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}

