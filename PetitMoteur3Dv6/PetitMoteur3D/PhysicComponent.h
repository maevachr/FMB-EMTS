#pragma once

#include "GameObject.h"
#include "PhysicManager.h"
#include <foundation\PxTransform.h>
#include "PxPhysicsAPI.forward.h"
#include <PxMaterial.h>
#include "SimulationManager.h"
#include "PhysX/Include/cooking/PxCooking.h"
#include "RenderComponent.h"
#include "CollisionFilter.h"

using namespace physx;

namespace PM3D
{
	class PhysicComponent : public Component
	{
	public:
		static constexpr char* typeId = "PhysicComponent";
		virtual const char* GetTypeId() { return "PhysicComponent"; }
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			PhysicManager::GetInstance().CreateComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			PhysicManager::GetInstance().RemoveComponent(this);
		}
	public:
		virtual void InitTerrainPhysic() = 0;
		virtual void AddActor() = 0;
		virtual void UpdateGoTransform() = 0;
	};

	class DynamicPhysicComponent : public PhysicComponent
	{
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			PhysicManager::GetInstance().CreateComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			PhysicManager::GetInstance().RemoveComponent(this);
		}
	private:
		physx::unique_ptr<PxMaterial> material;
		PxShape* actorShape;
		PxRigidDynamic *pxActor;

	public:
		physx::PxRigidDynamic * GetActor() { return pxActor; }
		virtual void AddActor()
		{
			SimulationManager::GetInstance().scene().addActor(*pxActor);
		}
		virtual void InitTerrainPhysic(){}
		virtual void UpdateGoTransform()
		{
			owner->SetTransform(pxActor->getGlobalPose());
		}
	public:
		void InitData(const PxGeometry& g, physx::unique_ptr<PxMaterial> m, const PxFilterData& filterData = PxFilterData{})
		{
			material = move(m);

			PxTransform transform = owner->GetTransform();

			PxTransform moveInPosition = physx::PxTransform::createIdentity();
			moveInPosition.p = transform.p;

			pxActor = SimulationManager::GetInstance().physics().createRigidDynamic(moveInPosition);
			pxActor->setGlobalPose(transform);

			actorShape = pxActor->createShape(g, *material);
			actorShape->setSimulationFilterData(filterData);
		}

		void InitMass(const PxReal& mass, const PxTransform& centerMass = PxTransform::createIdentity()){
			pxActor->setMass(mass);
			pxActor->setCMassLocalPose(centerMass);

		}	
	};

	class TerrainPhysicComponent : public PhysicComponent
	{
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			PhysicManager::GetInstance().CreateComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			PhysicManager::GetInstance().RemoveComponent(this);
		}
	private:
		physx::unique_ptr<PxMaterial> material;
		PxShape* actorShape;
		physx::PxRigidStatic *pxActor;
	
	public:
		physx::PxRigidStatic * GetActor() { return pxActor; }
		virtual void AddActor()
		{
			SimulationManager::GetInstance().scene().addActor(*pxActor);
		}
		virtual void InitTerrainPhysic()
		{
			PxPhysics &physics = SimulationManager::GetInstance().physics();
			material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.05f, 0.05f, 0.0f));

			RenderComponent::TerrainItems t = owner->As<RenderComponent>()->GetTerrainItems();

			PxTriangleMeshDesc meshDesc;
			vector<PxVec3> verts;
			vector<PxU32>  tri;

			meshDesc.points.count = t.nombreSommets;
			for (int i = 0; i < meshDesc.points.count; ++i)
			{
				verts.emplace_back(PxVec3{ t.ts[i].position.x,t.ts[i].position.y,t.ts[i].position.z });
			}
			meshDesc.points.stride = sizeof(PxVec3);
			meshDesc.points.data = &verts[0];

			meshDesc.triangles.count = t.nombreIndex / 3;
			meshDesc.triangles.stride = 3 * sizeof(PxU32);
			for (int i = 0; i < t.nombreIndex / 3; ++i)
			{
				tri.push_back(PxU32{ t.index[3 * i] });
				tri.push_back(PxU32{ t.index[3 * i + 2] });
				tri.push_back(PxU32{ t.index[3 * i + 1] });
			}
			meshDesc.triangles.data = &tri[0];

			PxDefaultMemoryOutputStream writeBuffer;
			bool status = SimulationManager::GetInstance().cooking->cookTriangleMesh(meshDesc, writeBuffer);

			PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			PxTriangleMesh * mesh = SimulationManager::GetInstance().physics().createTriangleMesh(readBuffer);

			pxActor = SimulationManager::GetInstance().physics().createRigidStatic(physx::PxTransform::createIdentity());
			pxActor->setGlobalPose(owner->GetTransform());

			actorShape = pxActor->createShape(PxTriangleMeshGeometry(mesh), *material);
			PxFilterData filterData;
			filterData.word0 = eACTOR_TERRAIN;
			filterData.word1 = eACTOR_PLAYER;
			actorShape->setSimulationFilterData(filterData);
		}
		virtual void UpdateGoTransform(){}
	};
}

