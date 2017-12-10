#pragma once
#include "PhysX/Include/PxPhysicsAPI.h"
#include "PxPhysicsAPI.forward.h"
#include <memory>

//=============================================================================
// CLASS SimulationManager
//=============================================================================

using namespace physx;


namespace PM3D
{
	class SimulationManager
	{
	public: // ISimulationManager
		static SimulationManager& GetInstance() {
			static SimulationManager singleton;
			return singleton;
		}

	protected:
		SimulationManager() = default;
		~SimulationManager() = default;

	public:
		physx::PxPhysics& physics();
		physx::PxScene& scene();

		void InitPhysX();
		void CustomizeSceneDesc(PxSceneDesc *aSceneDesc);
		static PxFilterFlags SimulationFilterShader(
			PxFilterObjectAttributes attributes0, PxFilterData filterData0,
			PxFilterObjectAttributes attributes1, PxFilterData filterData1,
			PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);
		void TerminatePhysX();
		const physx::unique_ptr<PxFoundation>& GetFoundation() { return _foundation; }
		const physx::unique_ptr<PxProfileZoneManager>& GetProfileZoneManager() { return _profileZoneManager; }
		const physx::unique_ptr<PxPhysics>& GetPhysics() { return _physics; };
		const physx::unique_ptr<PxDefaultCpuDispatcher>& GetDefaultCpuDispatcher() { return _cpuDispatcher; }
		const physx::unique_ptr<debugger::comm::PvdConnection>& GetPVDConnection() { return _visualDebuggerConnection; }

		void Update();
		

		PxCooking * cooking;

	private:
		physx::unique_ptr<PxFoundation> _foundation;
		physx::unique_ptr<PxProfileZoneManager> _profileZoneManager;

		physx::unique_ptr<PxPhysics> _physics;
		physx::unique_ptr<PxScene> _scene;

		physx::unique_ptr<PxDefaultCpuDispatcher> _cpuDispatcher;
		physx::unique_ptr<PxCudaContextManager> _cudaContextManager;
		physx::unique_ptr<debugger::comm::PvdConnection> _visualDebuggerConnection;
	};
}

