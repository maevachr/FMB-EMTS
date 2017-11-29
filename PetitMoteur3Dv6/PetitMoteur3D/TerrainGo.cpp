#include "stdafx.h"
#include "TerrainGo.h"
#include "RenderComponent.h"
#include "PhysicComponent.h"

namespace PM3D
{
	void PM3D::TerrainGo::OnSpawn(const PxTransform & _transform, GameObject * _parent)
	{
		GameObject::OnSpawn(_transform, _parent);

		//Set GameObjects


		//Set Components
		//-----RenderComponent
		RenderComponent* p = CreateComponent<RenderComponent>();
		p->GetMesh("terrain");

		//-----StaticPhysicComponent
		TerrainPhysicComponent* t = CreateComponent<TerrainPhysicComponent>();
	}

	void PM3D::TerrainGo::OnUnspawn()
	{
		GameObject::OnUnspawn();

		//Remove GameObjects
		std::for_each(std::begin(children), std::end(children), [](GameObject* go) {
			delete(go);
		});
		children.clear();
	}
}
