///\file Sync stepper implementation
///
/// Based on the physX3 sample stepper
#pragma once

#include "Common.h"
#include "PhysX3Util.h"

/// \brief Default Sync Stepper Base class
class Stepper
{
public:
	/// Stepper constructor
	Stepper() : mNbSubSteps(0)
	{}

	virtual bool	advance(PhysX3Util *physX3, physx::PxReal dt);
	virtual void	substepStrategy(const physx::PxReal stepSize, physx::PxU32 &substepCount, physx::PxReal &substepSize) = 0;
	virtual void	setSubStepper(const physx::PxReal stepSize, const physx::PxU32 maxSteps) {};

	physx::PxReal	getSimulationTime() {return mSimulationTime;}
	virtual void	reset() = 0;

protected:
	physx::PxReal	mSimulationTime;
	physx::PxU32	mNbSubSteps;
	physx::PxReal	mSubStepSize;
};

/// \brief FixedStepper implementation
///
/// Step size is fixed. PhysX will only step if dt >= stepSize
class FixedStepper : public Stepper
{
public:
	FixedStepper(const physx::PxReal subStepSize, const physx::PxU32 maxSubSteps) : Stepper(),
		mAccumulator(0),
		mFixedSubStepSize(subStepSize),mMaxSubSteps(maxSubSteps)
	{};

	virtual void	substepStrategy(const physx::PxReal stepSize, physx::PxU32 &substepCount, physx::PxReal &substepSize);
	virtual void	setSubStepper(const physx::PxReal stepSize, const physx::PxU32 maxSteps) { mFixedSubStepSize = stepSize; mMaxSubSteps = maxSteps;};

	virtual void	reset() {mAccumulator = 0.0f;};

	physx::PxReal	mAccumulator;
	physx::PxReal	mFixedSubStepSize;
	physx::PxU32	mMaxSubSteps;
};

/// \brief VariableStepper implementation
///
/// Step size is not fixed. PhysX will step if minSubStepSize < dt < maxSubstepSize
class VariableStepper : public Stepper
{
public:
	VariableStepper(const physx::PxReal minSubStepSize, const physx::PxReal maxSubStepSize, const physx::PxU32 maxSubSteps) : Stepper(),
		mAccumulator(0),
		mMinSubStepSize(minSubStepSize),
		mMaxSubStepSize(maxSubStepSize),
		mMaxSubSteps(maxSubSteps)
	{};

	virtual void			substepStrategy(const physx::PxReal stepSize, physx::PxU32 &substepCount, physx::PxReal &substepSize);

	virtual void			reset() {mAccumulator = 0.0f;};

protected:
			physx::PxReal	mAccumulator;
	const	physx::PxReal	mMinSubStepSize;
	const	physx::PxReal	mMaxSubStepSize;
	const	physx::PxU32	mMaxSubSteps;
};
