#pragma once

#include "GameObject.h"
#include "PhysicManager.h"
#include <foundation\PxTransform.h>
#include "PxPhysicsAPI.forward.h"
#include <PxMaterial.h>
#include "SimulationManager.h"
#include "PhysX/Include/cooking/PxCooking.h"
#include "RenderComponent.h"
#include "ICollisionHandler.h"
#include "PhysxVehicle.h"
#include "MoteurWindows.h"

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
		virtual void AddActor() {};
		virtual void UpdateGoTransform() {};
	};


	class PlateformePhysicComponent : public CollidingComponent
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
		void AddActor() override
		{
			pxActor->userData = static_cast<GameObject*>(GetOwner());
			SimulationManager::GetInstance().scene().addActor(*pxActor);
		}
		void UpdateGoTransform() override
		{
			//pxActor->addTorque(0.15*PxVec3{ 0, 0, 1 }, PxForceMode::eVELOCITY_CHANGE);
			pxActor->setAngularVelocity(3.0*PxVec3{ 0, 0, 1 });

			PxTransform localPose = pxActor->getGlobalPose();
			GameObject* parent = GetOwner()->GetParent();
			if (parent)
			{
				localPose = localPose * parent->GetWorldTransform().getInverse();
			}
			owner->SetTransform(localPose);
		}
	public:
		void InitData(const PxGeometry& g, physx::unique_ptr<PxMaterial> m)
		{

			material = move(m);

			PxTransform transform = owner->GetWorldTransform();

			PxTransform moveInPosition = physx::PxTransform::createIdentity();
			moveInPosition.p = transform.p;

			pxActor = SimulationManager::GetInstance().physics().createRigidDynamic(moveInPosition);
			pxActor->setGlobalPose(transform);

			pxActor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			pxActor->setMass(0);
			pxActor->setMassSpaceInertiaTensor(PxVec3{ 0,0,0 });

			actorShape = pxActor->createShape(g, *material);

			PxFilterData filterData;
			filterData.word0 = COLLISION_FLAG_PLATEFORME;
			filterData.word1 = COLLISION_FLAG_PLATEFORME_AGAINST;
			actorShape->setSimulationFilterData(filterData);
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
		void AddActor() override
		{
			pxActor->userData = static_cast<GameObject*>(GetOwner());
			SimulationManager::GetInstance().scene().addActor(*pxActor);
		}
		void UpdateGoTransform() override
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
		void InitData(const PxGeometry& g, physx::unique_ptr<PxMaterial> m, 
			const PxFilterData& filterData = PxFilterData{}, bool drivable = false)
		{
			material = move(m);

			PxTransform transform = owner->GetWorldTransform();

			PxTransform moveInPosition = physx::PxTransform::createIdentity();
			moveInPosition.p = transform.p;

			pxActor = SimulationManager::GetInstance().physics().createRigidDynamic(moveInPosition);
			pxActor->setGlobalPose(transform);
			actorShape = pxActor->createShape(g, *material);

			if (drivable)
			{
				physx::PxFilterData qryFilterData;
				qryFilterData.word3 = (PxU32)DRIVABLE_SURFACE;
				actorShape->setQueryFilterData(qryFilterData);
			}

			actorShape->setSimulationFilterData(filterData);
		}

		void InitMass(const PxReal& mass, const PxTransform& centerMass = PxTransform::createIdentity(), const PxVec3& inertiaTensor = { 1,1,1 }) {
			pxActor->setMass(mass);
			pxActor->setMassSpaceInertiaTensor(inertiaTensor);
			pxActor->setCMassLocalPose(centerMass);
		}
	};

	class StaticPhysicComponent : public CollidingComponent
	{
	private:
		GameObject* owner;
	public:
		virtual GameObject* GetOwner() const { return owner; }

	public:
		virtual void OnAttached(GameObject* _owner) override
		{
			owner = _owner;
			PhysicManager::GetInstance().CreateStaticComponent(this);
		}
		virtual void OnDetached() override
		{
			owner = nullptr;
			pxActor->release();
			PhysicManager::GetInstance().RemoveStaticComponent(this);
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
		void InitTriangleMeshPhysic()
		{
			PxPhysics &physics = SimulationManager::GetInstance().physics();
			material = physx::unique_ptr<PxMaterial>(physics.createMaterial(0.25f, 0.25f, 0.0f));

			NormalMesh::MeshData t = owner->As<RenderComponent>()->GetNormalMesh()->GetMeshData();

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

			physx::PxFilterData qryFilterData;
			qryFilterData.word3 = (PxU32)DRIVABLE_SURFACE;
			actorShape->setQueryFilterData(qryFilterData);

			PxFilterData filterData;
			filterData.word0 = COLLISION_FLAG_GROUND;
			filterData.word1 = COLLISION_FLAG_GROUND_AGAINST;
			actorShape->setSimulationFilterData(filterData);
		}
	};

	class VehiclePhysicComponent : public CollidingComponent
	{
	private:
		GameObject* owner;
		PxShape** buf;
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
		PxRigidDynamic* GetPxActor() {
			return pxActor;
		}
	private:
		physx::unique_ptr<PxMaterial> material;
		PxRigidDynamic *pxActor;
		PhysxVehicle physxVehicle;

	public:
		physx::PxRigidDynamic * GetActor() { return pxActor; }
		PhysxVehicle* GetVehicle() { return &physxVehicle; }
		virtual void AddActor()
		{
			pxActor->userData = static_cast<GameObject*>(GetOwner());
			SimulationManager::GetInstance().scene().addActor(*pxActor);
		}
		virtual void InitTerrainPhysic() {}

	private:
		bool isReverse() {
			PxVec3 upTruck = owner->GetTransform().q.rotate(PxVec3(0.0, 1.0, 0.0));
			return upTruck.z < 0.40;
		}

		float lastTime;
		int nbTic;

		bool isRepositioningTime() {
			float time = static_cast<float>(CMoteurWindows::GetInstance().GetTimeSpecific()) * 0.001f;
			if (time - lastTime < 0.5f) {
				++nbTic;
			}
			else {
				nbTic = 0;
			}
			lastTime = time;
			if (nbTic >= 200) {
				nbTic = 0;
				return true;
			}
			return false;
		}

	

	public:
		virtual void UpdateGoTransform()
		{
			if (isReverse()) {
				//show sprite
				if (isRepositioningTime()) {
					PxTransform newTransform = PxTransform::createIdentity();
					PxTransform transforTruck = owner->GetTransform();
					newTransform.p = PxVec3{ transforTruck.p.x, transforTruck.p.y, transforTruck.p.z + 3 };
					
					PxVec3 Up = transforTruck.q.rotate(PxVec3(0,1,0));
					PxVec3 Front = transforTruck.q.rotate(PxVec3(0, 0, 1));
					PxQuat original = PxQuat(0.707f, 0.f, 0.f, 0.707f);
					Front.z = 0;
					Front.getNormalized();
					float cos2t = Front.dot(original.rotate(PxVec3(0, 0, 1)));
					PxVec3 axis = original.rotate(PxVec3(0, 0, 1)).cross(Front).getNormalized()*(sqrt(-0.5f*(cos2t-1)));

					newTransform.q = PxQuat(axis.x,axis.y,axis.z,sqrt((cos2t+1)/2.0f))*original;

					PxVec3 newFront = newTransform.q.rotate(PxVec3(0, 0, 1));

					pxActor->setGlobalPose(newTransform);
					owner->SetTransform(newTransform);
				}
			}
			else {
				//dont show sprite
			}
			physxVehicle.stepPhysics();
			owner->SetTransform(pxActor->getGlobalPose());
			std::vector<GameObject*> child = owner->GetChildren();
			for (int i = 0; i < 4; i++) {
				child[i]->SetTransform(buf[i]->getLocalPose());
			}
		}
	public:
		void InitData(PxTransform startTransform)
		{
			buf = new PxShape*[5];
			pxActor = physxVehicle.initPhysics(startTransform);
			pxActor->getShapes(buf, 5);
		}

		void InitMass(const PxReal& mass, const PxTransform& centerMass = PxTransform::createIdentity()) {
			pxActor->setMass(mass);
			pxActor->setCMassLocalPose(centerMass);

		}
	};
}

