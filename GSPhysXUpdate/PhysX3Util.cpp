#include "Common.h"
#include "PhysX3Util.h"
#include "Stepper.h"

/// \brief PhysX3 Utility class constructor
///
/// Init PhysX system
PhysX3Util::PhysX3Util(void) : 
	mSDK(NULL),
	mFoundation(NULL),
	mProfileZoneManager(NULL),
	mCooking(NULL),
	mScene(NULL),
	mCpuDispatcher(NULL),
	mNbThreads(1),
	mRecordMemoryAllocations(true),
	mDeltaTime(0.016666660f),
	mUseFixedStepper(true),
	mSimulationTime(0.0f),
	mFixedStepper(new FixedStepper(mDeltaTime, 8)),
	mVariableStepper(new VariableStepper(1.0f / 80.0f, 1.0f / 40.0f, 8)),
	mScale(1 / 40),
	mDefaultGravity(physx::PxVec3(0.0f, -9.81f, 0.0f))
{
}

/// \brief PhysX3 Utility class destructor
///
/// Free occupied memory
PhysX3Util::~PhysX3Util(void)
{
}

/// \brief init physX engine
void PhysX3Util::Init(void)
{
}

/// \brief Release physX systems
void PhysX3Util::Release(void)
{
}

/// \brief Run Simulations
void PhysX3Util::Run(float deltaTime)
{
}

/// \brief Update properties
void PhysX3Util::Update(void)
{
}

/// \brief Get current stepper
Stepper *PhysX3Util::GetStepper(void)
{
	Stepper *stepper = mVariableStepper;
	if (mUseFixedStepper)
		stepper = mFixedStepper;
	
	return stepper;
}

/// \brief Set substepper properties for Fixed timestep
void PhysX3Util::SetSubStepper(const physx::PxReal &, const physx::PxU32, int method)
{
}

/// \brief Move a kinematic body
void PhysX3Util::MoveKinematic(ENTITY *entity, const physx::PxVec3 &disp)
{
}

/// \brief Update kinematic body to buffered position
void PhysX3Util::UpdateKinematics()
{
}