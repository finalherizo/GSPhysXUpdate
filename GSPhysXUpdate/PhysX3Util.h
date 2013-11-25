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
	void TransformRigidEntity(physx::PxRigidActor *);
	void CreateGroundPlane();
	void CreateKinematic(ENTITY *entity);
	void MoveKinematic(ENTITY *entity, const physx::PxVec3 &disp);
	void UpdateKinematics();

	/// \brief Populate a pre initialized 3dgs vector from a PxVec3
	///
	/// @param vec3 The original PxVec3 vector
	/// @param vec A pre initialized 3dgs vector
	/// @param scaled True if scale will be used in the calculus false otherwise
	/// @param swapped Specify if the axis needs to be swapped (y-z). default : true
	void PxVec3ToVec(physx::PxVec3 &vec3, VECTOR *vec, bool scaled = true, bool swapped = true)
	{
		physx::PxReal scale = 1;
		if (scaled)
		{
			scale = this->mScale;
		}
		if (vec)
		{
			physx::PxVec3 scaledVec = vec3 / scale;
			vec->x = _VAR(scaledVec.x);
			if (swapped == true)
			{
				vec->y = _VAR(scaledVec.z);
				vec->z = _VAR(scaledVec.y);
			}
			else
			{
				vec->y = _VAR(scaledVec.y);
				vec->z = _VAR(scaledVec.z);
			}
		}
	};

	/// \brief Populate a PxVec3 from 3dgs vector
	///
	/// @param vec3 The destination PxVec3 vector
	/// @param vec The source 3dgs vector
	/// @param scaled True if scale will be used in the calculus false otherwise
	/// @param swapped Specify if the axis needs to be swapped (y-z). default : true
	void PxVec3ForVec(physx::PxVec3 &vec3, VECTOR *vec, bool scaled = true, bool swapped = true)
	{
		physx::PxReal scale = 1;
		if (scaled)
		{
			scale = this->mScale;
		}
		vec3 = physx::PxVec3(0);
		if (vec)
		{
			vec3.x = _FLOAT(vec->x) * scale;
			if (swapped == true)
			{
				vec3.y = _FLOAT(vec->z) * scale;
				vec3.z = _FLOAT(vec->y) * scale;
			}
			else
			{
				vec3.y = _FLOAT(vec->y) * scale;
				vec3.z = _FLOAT(vec->z) * scale;
			}
		}
	};

	/// \brief Populate a pre initialized 3dgs vector from a PxVec3 representing a torque
	///
	/// Torque is in radian 3dgs vector is in degree
	/// @param vec3 The original PxVec3 vector
	/// @param vec A pre initialized 3dgs vector
	/// @param swapped Specify if the axis needs to be swapped (y-z). default : true
	void PxTorqueToVec(physx::PxVec3 &vec3, VECTOR *vec, bool swapped = true)
	{
		if (vec)
		{
			vec->x = _VAR(-CONV_RADTODEG(vec3.x));
			if (swapped == true)
			{
				vec->y = _VAR(-CONV_RADTODEG(vec3.z));
				vec->z = _VAR(-CONV_RADTODEG(vec3.y));
			}
			else
			{
				vec->y = _VAR(-CONV_RADTODEG(vec3.y));
				vec->z = _VAR(-CONV_RADTODEG(vec3.z));
			}
		}
	};

	/// \brief Populate a PxVec3 representing a torque from 3dgs vector
	///
	/// Torque is in radian 3dgs vector is in degree
	/// @param vec3 The destination PxVec3 vector
	/// @param vec The source 3dgs vector
	/// @param scaled True if scale will be used in the calculus false otherwise
	/// @param swapped Specify if the axis needs to be swapped (y-z). default : true
	void PxTorqueForVec(physx::PxVec3 &vec3, VECTOR *vec, bool swapped = true)
	{
		vec3 = physx::PxVec3(0);
		if (vec)
		{
			vec3.x = -CONV_DEGTORAD(_FLOAT(vec->x));
			if (swapped == true)
			{
				vec3.y = -CONV_DEGTORAD(_FLOAT(vec->z));
				vec3.z = -CONV_DEGTORAD(_FLOAT(vec->y));
			}
			else
			{
				vec3.y = -CONV_DEGTORAD(_FLOAT(vec->y));
				vec3.z = -CONV_DEGTORAD(_FLOAT(vec->z));
			}
		}
	};

	/// \brief Create a quaternion from a 3dgs angle
	///
	/// @param q The quaternion to be created
	/// @para ang The source 3DGS angle
	void PxQuatForAng(physx::PxQuat &q, ANGLE *ang)
	{
		float mat[16];
		ANGLE tempAngle;
		vec_set((VECTOR *)&tempAngle, (VECTOR*)ang);

		ang_to_matrix(&tempAngle,&(mat[0]));

		// Transform to pxMat
		physx::PxMat33 m(physx::PxVec3(mat[0], mat[1], mat[2]), physx::PxVec3(mat[4], mat[5], mat[6]), physx::PxVec3(mat[8], mat[9], mat[10]));
		q = physx::PxQuat(m);
	};

	/// \brief Populate a 3DGS angle from a quaternion
	///
	/// @param q The source quaternion
	/// @para ang The 3DGS angle to be populated
	void PxQuatToAng(physx::PxQuat &q, ANGLE *ang)
	{
		physx::PxMat33 m = physx::PxMat33(q);
		float mat[16];
		mat[0]  = m[0][0]; mat[1]  = m[0][1]; mat[2]  = m[0][2]; mat[3]  = 0;
		mat[4]  = m[1][0]; mat[5]  = m[1][1]; mat[6]  = m[1][2]; mat[7]  = 0;
		mat[8]  = m[2][0]; mat[9]  = m[2][1]; mat[10] = m[2][2]; mat[11] = 0;
		mat[12] = 0; mat[13] = 0; mat[14] = 0; mat[15] = 1;
		// Rotate entity
		ang_for_matrix(ang, &mat[0]);
	};

	/// \brief Get entity's extent independent of the scale
	///
	/// @param ent
	/// @param min
	/// @param max
	void EntGetMinMax(ENTITY *ent, physx::PxVec3 &min, physx::PxVec3 &max)
	{
		VECTOR vMin, vMax;
		vec_for_min(&vMin, ent);
		vec_for_max(&vMax, ent);

		min = physx::PxVec3(_FLOAT(vMin.x), _FLOAT(vMin.z), _FLOAT(vMin.y)) * this->mScale;
		max = physx::PxVec3(_FLOAT(vMax.x), _FLOAT(vMax.z), _FLOAT(vMax.y)) * this->mScale;
	}
};

