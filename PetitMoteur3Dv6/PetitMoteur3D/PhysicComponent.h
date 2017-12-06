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
#include "ICollisionHandler.h"

using namespace physx;

namespace PM3D
{

	class CollidingComponent :  public Component
	{
	public:
		static constexpr char* typeId = "CollidingComponent";
		virtual const char* GetTypeId() { return "CollidingComponent"; }
	protected:
		std::unique_ptr<ICollisionHandler> handler;
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) = 0;
		virtual void OnDetached() = 0;
	public:
		virtual void OnContact(const physx::PxContactPair &aContactPair)
		{
			if (handler)
			{
				handler->OnContact(aContactPair);
			}
		}
		virtual void OnTrigger(bool triggerEnter, physx::PxShape *actorShape, physx::PxShape *contactShape)
		{
			if (handler)
			{
				handler->OnTrigger(triggerEnter, actorShape, contactShape);
			}
		}
		virtual void SetHandler(std::unique_ptr<ICollisionHandler> iHandler)
		{
			handler = move(iHandler);
		}
	};


	class DynamicPhysicComponent : public CollidingComponent
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
			pxActor->release();
			PhysicManager::GetInstance().RemoveComponent(this);
		}
	private:
		physx::unique_ptr<PxMaterial> material;
		PxShape* actorShape;
		PxRigidDynamic *pxActor;

	public:
		physx::PxRigidDynamic * GetActor() { return pxActor; }
		void AddActor()
		{
			pxActor->userData = static_cast<GameObject*>(GetOwner());
			SimulationManager::GetInstance().scene().addActor(*pxActor);
		}
		void UpdateGoTransform()
		{
			PxTransform localPose = pxActor->getGlobalPose();
			GameObject* parent = GetOwner()->GetParent();
			if (parent)
			{
				localPose = localPose * parent->GetWorldTransform().getInverse() ;
			}
			owner->SetTransform(localPose);	
		}
	public:
		void InitData(const PxGeometry& g, physx::unique_ptr<PxMaterial> m, const PxFilterData& filterData = PxFilterData{})
		{
			material = move(m);

			PxTransform transform = owner->GetWorldTransform();

			PxTransform moveInPosition = physx::PxTransform::createIdentity();
			moveInPosition.p = transform.p;

			pxActor = SimulationManager::GetInstance().physics().createRigidDynamic(moveInPosition);
			pxActor->setGlobalPose(transform);

			actorShape = pxActor->createShape(g, *material);
			actorShape->setSimulationFilterData(filterData);
		}

		void InitMass(const PxReal& mass, const PxTransform& centerMass = PxTransform::createIdentity(), const PxVec3& inertiaTensor = { 1,1,1 }) {
			pxActor->setMass(mass);
			pxActor->setMassSpaceInertiaTensor(inertiaTensor);
			pxActor->setCMassLocalPose(centerMass);
		}
	};

	class TerrainPhysicComponent : public CollidingComponent
	{
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			PhysicManager::GetInstance().CreateTerrain(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			pxActor->release();
			PhysicManager::GetInstance().RemoveTerrain();
		}
	private:
		physx::unique_ptr<PxMaterial> material;
		PxShape* actorShape;
		physx::PxRigidStatic *pxActor;

	public:
		physx::PxRigidStatic * GetActor() { return pxActor; }
		void AddActor()
		{
			pxActor->userData = static_cast<GameObject*>(GetOwner());
			SimulationManager::GetInstance().scene().addActor(*pxActor);
		}
		void InitTerrainPhysic()
		{
			PxPhysics &physics = SimulationManager::GetInstance().physics();
			material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.05f, 0.05f, 0.0f));

			NormalMesh::TerrainItems t = owner->As<RenderComponent>()->GetNormalMesh()->GetTerrainItems();

			PxTriangleMeshDesc meshDesc;
			vector<PxVec3> verts;
			vector<PxU32>  tri;

			meshDesc.points.count = t.nombreSommets;
			for (unsigned int i = 0; i < meshDesc.points.count; ++i)
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
	};
}

