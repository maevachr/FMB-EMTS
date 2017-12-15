#pragma once

/*
* Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

// ****************************************************************************
// This snippet illustrates simple use of PxVehicleDrive4W.
//
// It creates a vehicle on a plane and then controls the vehicle so that it performs a 
// number of choreographed manoeuvres such as accelerate, reverse, brake, handbrake, and turn.

// It is a good idea to record and playback with pvd (PhysX Visual Debugger).
// ****************************************************************************
#include "StdAfx.h"

#include "PhysX/Include/PxPhysicsAPI.h"
#include "PhysX/Include/vehicle/PxVehicleUtil.h"
#include "PhysX/Include/vehicle/PxVehicleSDK.h"
#include "VehicleRaycast.h"
#include "VehicleFilterShader.h"
#include "VehicleTireFriction.h"
#include "VehicleCreate.h"

using namespace physx;

class PhysxVehicle {
public:
	enum DriveMode
	{
		eDRIVE_MODE_ACCEL_FORWARDS = 0,
		eDRIVE_MODE_ACCEL_REVERSE,
		eDRIVE_MODE_HARD_TURN_LEFT,
		eDRIVE_MODE_HANDBRAKE_TURN_LEFT,
		eDRIVE_MODE_HARD_TURN_RIGHT,
		eDRIVE_MODE_HANDBRAKE_TURN_RIGHT,
		eDRIVE_MODE_BRAKE,
		eDRIVE_MODE_NONE
	};

	VehicleDesc initVehicleDesc();
	void startAccelerateForwardsMode();
	void startAccelerateForwardsSecond();
	void startAccelerateReverseMode();
	void startBrakeMode();
	void startTurnHardLeftMode();
	void startTurnHardRightMode();
	void startHandbrakeTurnLeftMode();
	void startHandbrakeTurnRightMode();
	void startHandbrakeMode();
	void releaseHandbrake();
	void releaseSteering();
	void releaseAccelerate();
	void releaseAllControls();
	PxRigidDynamic* initPhysics(PxTransform startTransform);
	void incrementDrivingMode(const PxF32 timestep);
	void stepPhysics();
	void cleanupPhysics();
	void keyPress(const char key, const PxTransform& camera);

	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

	PxFoundation*			gFoundation = NULL;
	PxPhysics*				gPhysics = NULL;

	PxDefaultCpuDispatcher*	gDispatcher = NULL;
	PxScene*				gScene = NULL;

	PxCooking*				gCooking = NULL;

	PxMaterial*				gMaterial = NULL;

	PxVisualDebuggerConnection* gConnection = NULL;

	VehicleSceneQueryData*	gVehicleSceneQueryData = NULL;
	PxBatchQuery*			gBatchQuery = NULL;

	PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;

	PxRigidStatic*			gGroundPlane = NULL;
	PxVehicleDrive4W*		gVehicle4W = NULL;

	bool					gIsVehicleInAir = true;

	const static PxF32 gSteerVsForwardSpeedData[];
	const static PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable;
	const static DriveMode gDriveModeOrder[];
	const static PxVehicleKeySmoothingData gKeySmoothingData;
	const static PxVehiclePadSmoothingData gPadSmoothingData;
	PxVehicleDrive4WRawInputData gVehicleInputData;

	PxF32					gVehicleModeLifetime = 4.0f;
	PxF32					gVehicleModeTimer = 0.0f;
	PxU32					gVehicleOrderProgress = 0;
	bool					gVehicleOrderComplete = false;
	bool					gMimicKeyInputs = false;
private:
};