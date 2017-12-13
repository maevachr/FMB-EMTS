#include "stdafx.h"

#include "SimulationManager.h"
#include <iostream>
#include "PhysX/Include/extensions/PxDefaultAllocator.h"
#include "PhysX/Include/extensions/PxDefaultErrorCallback.h"
#include "PhysX/Include/foundation/PxFoundation.h"
#include "Moteur.h"
#include <chrono>
#include "PhysX/Include/cooking/PxCooking.h"
#include "GameObject.h"
#include "PhysicComponent.h"

using namespace physx;
using namespace std::chrono;


namespace PM3D
{
	class SimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override
		{}

		virtual void onWake(PxActor** actors, PxU32 count) override
		{}

		virtual void onSleep(PxActor** actors, PxU32 count) override
		{}

		virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override
		{
			GameObject* actor0 = nullptr;
			if (!(pairHeader.flags & PxContactPairHeaderFlag::eREMOVED_ACTOR_0))
				actor0 = static_cast<GameObject*>(pairHeader.actors[0]->userData);

			GameObject *actor1 = nullptr;
			if (!(pairHeader.flags & PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
				actor1 = static_cast<GameObject*>(pairHeader.actors[1]->userData);


			for (int i = 0; i < (int)nbPairs; ++i)
			{
				if (actor0)
					actor0->As<CollidingComponent>()->OnContact(pairs[i]);

				if (actor1)
					actor1->As<CollidingComponent>()->OnContact(pairs[i]);
			}
		}

		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override
		{
			for (int i = 0; i < (int)count; ++i)
			{
				bool triggerEnter = false;
				if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
					triggerEnter = true;
				else if (pairs->status == PxPairFlag::eNOTIFY_TOUCH_LOST)
					triggerEnter = false;
				else
					continue;

				GameObject *trigger = nullptr;
				if (!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER))
					trigger = static_cast<GameObject*>(pairs[i].triggerShape->getActor()->userData);

				GameObject *other = nullptr;
				if (!(pairs->flags & PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
					other = static_cast<GameObject*>(pairs[i].otherShape->getActor()->userData);

				if (trigger)
					trigger->As<CollidingComponent>()->OnTrigger(triggerEnter, pairs[i].triggerShape, other ? pairs[i].otherShape : nullptr);

				if (other)
					other->As<CollidingComponent>()->OnTrigger(triggerEnter, pairs[i].otherShape, trigger ? pairs[i].triggerShape : nullptr);
			}
		}

	};


	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static SimulationEventCallback gDefaultSimulationCallback;


	void SimulationManager::InitPhysX()
	{
		_foundation = physx::unique_ptr<PxFoundation>(
			PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback));
		if (!_foundation)
			std::cerr << "PxCreateFoundation failed!";

		PxTolerancesScale scale;
		scale.length = 1;
		scale.speed = 9, 8;
		scale.mass = 1000;
		cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, PxCookingParams(scale));

		bool recordMemoryAllocations = true;
		_profileZoneManager = physx::unique_ptr<PxProfileZoneManager>(
			&PxProfileZoneManager::createProfileZoneManager(_foundation.get())
			);
		if (!_profileZoneManager)
			std::cerr << "PxProfileZoneManager::createProfileZoneManager failed";


		_physics = physx::unique_ptr<PxPhysics>(
			PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation,
				scale, recordMemoryAllocations, _profileZoneManager.get()));
		if (!_physics)
			std::cerr << "PxCreatePhysics failed";


		if (!PxInitExtensions(*_physics))
			std::cerr << "PxInitExtensions failed";


		if (_physics->getPvdConnectionManager() != nullptr)
		{
			PxVisualDebuggerConnectionFlags connectionFlags(PxVisualDebuggerExt::getAllConnectionFlags());
			_visualDebuggerConnection = physx::unique_ptr<debugger::comm::PvdConnection>(
				PxVisualDebuggerExt::createConnection(_physics->getPvdConnectionManager(),
					"localhost", 5425, 100, connectionFlags));
			if (_visualDebuggerConnection == nullptr)
				std::cerr << "    NOT CONNECTED!\n";
			else
				std::cerr << "    CONNECTED!\n";
		}



		//-------------------------------------------------------------
		// create the scene
		PxSceneDesc sceneDesc(_physics->getTolerancesScale());
		CustomizeSceneDesc(&sceneDesc);

		if (!sceneDesc.cpuDispatcher)
		{
			_cpuDispatcher = physx::unique_ptr<PxDefaultCpuDispatcher>(PxDefaultCpuDispatcherCreate(1));
			if (!_cpuDispatcher)
				std::cerr << "PxDefaultCpuDispatcherCreate failed!";
			sceneDesc.cpuDispatcher = _cpuDispatcher.get();
		}
		if (!sceneDesc.filterShader)
			sceneDesc.filterShader = PxDefaultSimulationFilterShader;

		_scene = physx::unique_ptr<PxScene>(_physics->createScene(sceneDesc));
		if (!_scene)
			std::cerr << "createScene failed!";

		_scene->setSimulationEventCallback(&gDefaultSimulationCallback);
	}

	void SimulationManager::CustomizeSceneDesc(PxSceneDesc *aSceneDesc)
	{
		aSceneDesc->gravity = PxVec3(0.0f, 0.0f, -15.8f);
		//aSceneDesc->gravity = PxVec3(0.0f, 0.0f, 0.0f);
		aSceneDesc->filterShader = &SimulationFilterShader;
	}

	PxFilterFlags SimulationManager::SimulationFilterShader(
		PxFilterObjectAttributes attributes0, PxFilterData filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		// let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eDEFAULT;
		}
		// generate contacts for all that were not filtered above
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
		{
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			return PxFilterFlag::eDEFAULT;
		}

		return PxFilterFlag::eSUPPRESS;
	}

	void SimulationManager::TerminatePhysX()
	{
		_scene.reset();
		_cpuDispatcher.reset();
		_visualDebuggerConnection.reset();

		if (_physics)
		{
			PxCloseExtensions();
			_physics.reset();
		}

		_cudaContextManager.reset();

		_profileZoneManager.reset();
		_foundation.reset();
	}

	void SimulationManager::Update()
	{
		duration<double, std::milli> remainingToSimulate(1000 / PM3D::IMAGESPARSECONDE);

		auto durationStep = 100ms;
		while (remainingToSimulate > 0ms)
		{
			if (remainingToSimulate > durationStep)
			{
				_scene->simulate(duration_cast<duration<PxReal>>(durationStep).count());
				remainingToSimulate -= durationStep;
			}
			else
			{
				_scene->simulate(duration_cast<duration<PxReal>>(remainingToSimulate).count());
				remainingToSimulate = 0ms;
			}
			_scene->fetchResults(true);
		}


	}

	physx::PxPhysics& SimulationManager::physics()
	{
		return *_physics;
	}

	physx::PxScene& SimulationManager::scene()
	{
		return *_scene;
	}
}