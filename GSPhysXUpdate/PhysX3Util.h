#pragma once

#include "Common.h"

class Stepper;
class FixedStepper;
class VariableStepper;

/// \brief PhysX3 Utility implementation
class PhysX3Util
{
public:
	physx::PxPhysics *mSDK;
	physx::PxFoundation *mFoundation;
	physx::PxProfileZoneManager *mProfileZoneManager;
	physx::PxCooking *mCooking;
	physx::PxScene *mScene;
	physx::PxDefaultCpuDispatcher *mCpuDispatcher;

	physx::PxDefaultAllocator mDefaultAllocatorCallback;
	physx::PxDefaultErrorCallback mDefaultErrorCallback;

	int mNbThreads;
	bool mRecordMemoryAllocations;

	physx::PxMaterial *mDefaultMaterial;

	float mScale;

	physx::PxVec3 mDefaultGravity;

	private:
	// Simulation vars
	physx::PxReal			mDeltaTime;			// Last delta used
	physx::PxReal			mSimulationTime;
	bool			mUseFixedStepper;
	FixedStepper	*mFixedStepper;
	VariableStepper	*mVariableStepper;

#ifdef _DEBUG	
	bool mUsePVD;

	//PVD Connection
	physx::debugger::comm::PvdConnection *mPvdConnection;
#endif

public:
	PhysX3Util(void);
	~PhysX3Util(void);

	void Init(void);
	void Release(void);
	void Run(float deltaTime);
	void Update(void);
	Stepper *GetStepper(void);
	void SetSubStepper(const physx::PxReal &, const physx::PxU32, int method);
	physx::PxReal GetDeltaTime() {return mDeltaTime;};
	void SetDeltaTime(physx::PxReal dt) {mDeltaTime = dt;};
	void MoveKinematic(ENTITY *entity, const physx::PxVec3 &disp);
	void UpdateKinematics();
};

