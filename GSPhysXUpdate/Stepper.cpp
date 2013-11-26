#include "Stepper.h"
#include "Common.h"

/// \brief Advance stepper
/// \param physX3 {\link PhysX3Util} instance
/// \param dt
bool Stepper::advance(PhysX3Util *physX3, physx::PxReal dt)
{
	// Quit if no physX container is attached
	if (physX3 == NULL || physX3->mScene == NULL)
		return false;

	physx::PxScene *scene = physX3->mScene;

	// Call strategy
	substepStrategy(dt, mNbSubSteps, mSubStepSize);

	// Quit if no substep
	if (mNbSubSteps == 0) return false;
	
	physx::PxU32 currentSubstep = 0;
	
	// Run simulation on substeps
	// Simulate scene 
	// Move character controllers
	for (currentSubstep = 0; currentSubstep < mNbSubSteps; currentSubstep++)
	{
		physX3->UpdateKinematics();

		scene->simulate(mSubStepSize);
		scene->fetchResults(true);
	}

	return true;
}

/// \brief Setup FixedStepper SubstepStrategy
///
/// \param stepSize
/// \param subStepCountn
/// \param substepSize
void FixedStepper::substepStrategy(const physx::PxReal stepSize, physx::PxU32 &substepCount, physx::PxReal &substepSize)
{
	physx::PxReal frameTime = stepSize; // Frame time
	if (frameTime > mFixedSubStepSize * 2)
		frameTime = mFixedSubStepSize * 2;

	mAccumulator += frameTime;

	substepSize = mFixedSubStepSize; // dt
	substepCount = physx::PxMax((physx::PxU32)(mAccumulator / mFixedSubStepSize), physx::PxU32(0));

	mAccumulator -= physx::PxReal(substepCount)*substepSize;

	// Alpha calculations
	physx::PxReal alpha = mAccumulator / mFixedSubStepSize;
	//printf("a(%.2f)\tf(%.3f)\ts(%d)\n", alpha, mFixedSubStepSize, substepCount);
}

/// \brief Setup Variable Stepper SubstepStrategy
///
/// @param stepSize
/// @param subStepCountn
/// @param substepSize
void VariableStepper::substepStrategy(const physx::PxReal stepSize, physx::PxU32& substepCount, physx::PxReal& substepSize)
{
	if(mAccumulator > mMaxSubStepSize)
		mAccumulator = 0.0f;

	// don't step less than the min step size, just accumulate
	mAccumulator  += stepSize;
	if(mAccumulator < mMinSubStepSize)
	{
		substepCount = 0;
		return;
	}

	substepCount = physx::PxMin(physx::PxU32(physx::PxCeil(mAccumulator/mMaxSubStepSize)), mMaxSubSteps);
	substepSize = physx::PxMin(mAccumulator/substepCount, mMaxSubStepSize);

	mAccumulator -= physx::PxReal(substepCount)*substepSize;
}
