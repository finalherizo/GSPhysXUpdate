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
	mScale(1 / 40.0f),
	mDefaultGravity(physx::PxVec3(0.0f, -9.81f, 0.0f)),
	mUseActiveTransforms(false)
{
	#ifdef _DEBUG
		mUsePVD = true;

		// PVD Connection instance
		mPvdConnection = NULL;
	#endif
}

/// \brief PhysX3 Utility class destructor
///
/// Free occupied memory
PhysX3Util::~PhysX3Util(void)
{
	Release();
}

/// \brief init physX engine
void PhysX3Util::Init(void)
{
	if (mScene)
	{
		return;
	}

	// Create foundation
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
	if (NULL == mFoundation)
	{
		error("PxCreateFoundation faled!");
		return;
	}

	// Create zone anager
	mProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(mFoundation);
	if (NULL == mProfileZoneManager)
	{
		error("PxProfileZoneManager::createProfileZoneManager failed!");
		return;
	}

	// Create SDK
	mSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *(mFoundation), physx::PxTolerancesScale(), mRecordMemoryAllocations, mProfileZoneManager);
	if (NULL == mSDK)
	{
		error("PxCreatePhysics failed!");
		return;
	}

	// Create cooking
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *(mFoundation), physx::PxCookingParams(mSDK->getTolerancesScale()));
	if (NULL == mCooking)
	{
		error("PxCreateCooking failed!");
		return;
	}

	// Init extensions
	if (false == PxInitExtensions(*(this->mSDK)))
		error("PxInitExtensions failed!");
	
	// Set simulation callback

	// Create scene
	physx::PxSceneDesc sceneDesc(mSDK->getTolerancesScale());
	sceneDesc.gravity = mDefaultGravity;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
	//sceneDesc.simulationEventCallback = NULL;
	//sceneDesc.filterCallback = NULL;
	
	// Create cpu Dispatcher
	mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(this->mNbThreads);
	if (NULL == mCpuDispatcher)
	{
		error("PxDefaultCpuDispatcherCreate failed!");
		return;
	}
	sceneDesc.cpuDispatcher = this->mCpuDispatcher;
	
	mScene = mSDK->createScene(sceneDesc);
	if (NULL == mScene)
	{
		error("createScene failed!");
		return;
	}

	// Create default material
	mDefaultMaterial = this->mSDK->createMaterial(0.6f, 0.4f, 0.1f);
	if (NULL == mDefaultMaterial)
	{
		error("createMaterial failed!");
		return;
	}

#ifdef _DEBUG
	// Create PVD
	if (mUsePVD && NULL != mSDK->getPvdConnectionManager())
	{
		// Setup connexion
		const char* pvd_host_ip = "127.0.0.1";
		int			port		= 5425;
		unsigned int timeout	= 100;

		physx::PxVisualDebuggerConnectionFlags connectionFlags = physx::PxVisualDebuggerExt::getAllConnectionFlags();

		// Try to connect
		mPvdConnection = physx::PxVisualDebuggerExt::createConnection(mSDK->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);
	}
#endif
}

/// \brief Release physX systems
void PhysX3Util::Release(void)
{
	#ifdef _DEBUG
	// Close PVD
	if (this->mUsePVD && mPvdConnection != NULL)
	{
		this->mPvdConnection->release();
		this->mPvdConnection = NULL;
	}
#endif

	// Free PhysX objects
	// Release SDK
	PX_RELEASE(mSDK)
	
	// Release profile zone
	PX_RELEASE(mProfileZoneManager)
	
	// Release coocking
	PX_RELEASE(mCooking)

	// Release extensions
	PxCloseExtensions();
	
	// Release CPU dispatcher
	PX_RELEASE(mCpuDispatcher)
	
	// Release Foundation
	PX_RELEASE(mFoundation)
	
	// Free fixed Members
	PX_DELETE(mFixedStepper)

	// Free variable stepper method
	PX_DELETE(mVariableStepper)
}

/// \brief Run Simulations
void PhysX3Util::Run(float deltaTime)
{
	// Store last used delta to allow calling funtion with 0
	if (deltaTime > 0)
	{
		mDeltaTime = deltaTime;
	}

	// Advance simulation stepper
	Stepper *stepper = GetStepper();
	bool advanced = stepper->advance(this, mDeltaTime);
	
	if (advanced)
		Update();
}

/// \brief Update properties
void PhysX3Util::Update(void)
{
	if (mUseActiveTransforms)
	{
		// Retrieve array of actors that moved
		physx::PxU32 nbActiveTransforms;
		const physx::PxActiveTransform* activeTransforms = mScene->getActiveTransforms(nbActiveTransforms, physx::PX_DEFAULT_CLIENT);
	
		if (nbActiveTransforms)
		{
			// Transform each actor
			for (physx::PxU32 i = 0; i < nbActiveTransforms; i++)
			{
				physx::PxActor *actor = activeTransforms[i].actor;
			
				// Skip actors owned by foreign clients
				if (physx::PX_DEFAULT_CLIENT == actor->getOwnerClient())
				{
					// Treat actor differently depending on actor type
					// Update only if shape is static_rigid or static_dynamic
					switch(actor->getType())
					{
					case physx::PxActorType::eRIGID_STATIC:
					case physx::PxActorType::eRIGID_DYNAMIC:
					
						// Update entities from rigid actor
						TransformRigidEntity((physx::PxRigidActor *)actor);
						break;

		#if PX_USE_PARTICLE_SYSTEM_API
					case physx::PxActorType::ePARTICLE_SYSTEM:
					case physx::PxActorType::ePARTICLE_FLUID:
		#endif

		#if PX_USE_CLOTH_API
					case physx::PxActorType::eCLOTH:
		#endif
					default:
						break;
					}
				}
			}
		}
	}
	else
	{
		int nbActors = mScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);
		physx::PxActor **buffer = new physx::PxActor*[nbActors];
		mScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC, buffer, nbActors);

		for (int i = 0; i < nbActors; i++)
		{
			TransformRigidEntity((physx::PxRigidActor *)buffer[i]);
		}

		delete[] buffer;
	}
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
void PhysX3Util::SetSubStepper(const physx::PxReal &stepSize, const physx::PxU32 maxIter, int method)
{
	// Quit if stepper method is unknown
	if (method != PX_TIMESTEP_FIXED && method != PX_TIMESTEP_VARIABLE)
	{
		return;
	}

	mFixedStepper->setSubStepper(stepSize, maxIter);
	mVariableStepper->setSubStepper(stepSize, maxIter);
	
	// Reset stepper
	mFixedStepper->reset();
	mVariableStepper->reset();

	if (method == PX_TIMESTEP_FIXED)
	{
		mUseFixedStepper = true;
	}
	else
	{
		mUseFixedStepper = false;
	}
}

/// \brief Create ground plane
void PhysX3Util::CreateGroundPlane()
{
}

/// \brief Create box kinematic from entity
physx::PxRigidDynamic *PhysX3Util::CreateKinematic(ENTITY *entity)
{
	// Current entity transform
	physx::PxQuat entityRotation;
	physx::PxVec3 vecMin, vecMax, entityPosition, halfExtents, scale;
	physx::PxTransform transform;

	// Transform entity rotation to quaternion
	PxQuatForAng(entityRotation, (ANGLE*)&(entity->pan));

	// Scale entity position
	PxVec3ForVec(entityPosition, (VECTOR*)&(entity->x));
	PxVec3ForVec(scale, (VECTOR*)&(entity->scale_x), false, true);

	// Create dynamic or rigid actor based on body type
	physx::PxRigidDynamic *boxActor = NULL;
	physx::PxShape *boxShape = NULL;
	transform = physx::PxTransform(entityPosition, entityRotation);
	
	// Create body based on provided type
	boxActor = mSDK->createRigidDynamic(transform);

	// Get entity extents for box shape
	EntGetMinMax(entity, vecMin, vecMax);
	halfExtents = (vecMax - vecMin) * 0.5;
	// Scale
	halfExtents = halfExtents.multiply(scale);
	
	boxShape = boxActor->createShape(physx::PxBoxGeometry(halfExtents), *mDefaultMaterial);
	boxActor->userData = new KineSettings(entity->link.index, transform);
	
	entity->body = boxActor;

	physx::PxRigidBodyExt::updateMassAndInertia(*(physx::PxRigidBody *)boxActor, 1.0f);
	boxActor->setRigidDynamicFlag(physx::PxRigidDynamicFlag::eKINEMATIC, true);

	mScene->addActor(*boxActor);

	return boxActor;
}

/// \brief Move a kinematic body
void PhysX3Util::MoveKinematic(ENTITY *entity, VECTOR *disp)
{
	// Get entity's actor
	physx::PxRigidDynamic *kine = (physx::PxRigidDynamic *) entity->body;

	if (NULL == kine)
		return;

	KineSettings *settings = (KineSettings *)kine->userData;

	// Create global position from absolute and relative vectors
	physx::PxVec3 localPos;
	physx::PxTransform actorGlobalPose(physx::PxIdentity);

	PxVec3ForVec(localPos, disp);
		
	// Transform relative vector to global based on actor type
	actorGlobalPose = settings->pose;

	// Get rotated local pose
	localPos = actorGlobalPose.q.rotate(localPos);

	// Add Localpose to Global pose
	actorGlobalPose.p += localPos;

	kine->setKinematicTarget(actorGlobalPose);
	// Save back settings
	settings->pose = actorGlobalPose;
}

/// \brief Move a kinematic body
void PhysX3Util::RotateKinematic(ENTITY *entity, ANGLE *disp)
{
	// Get entity's actor
	physx::PxRigidDynamic *kine = (physx::PxRigidDynamic *) entity->body;

	if (NULL == kine)
		return;

	KineSettings *settings = (KineSettings *)kine->userData;

	// Create global position from absolute and relative vectors
	physx::PxQuat localRot;
	physx::PxTransform actorGlobalPose(physx::PxIdentity);

	PxQuatForAng(localRot, disp);
		
	// Transform relative vector to global based on actor type
	actorGlobalPose = settings->pose;

	// Add Localpose to Global pose
	actorGlobalPose.q *= localRot;

	kine->setKinematicTarget(actorGlobalPose);
	// Save back settings
	settings->pose = actorGlobalPose;
}

/// \brief Move kinematics every frame
void PhysX3Util::UpdateKinematics(physx::PxReal factor)
{
	int nbActors = mScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);
	physx::PxActor **buffer = new physx::PxActor*[nbActors];
	mScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC, buffer, nbActors);

	for (int i = 0; i < nbActors; i++)
	{
		KineSettings *settings = (KineSettings *)buffer[i]->userData;	
		physx::PxRigidDynamic *kine = (physx::PxRigidDynamic *)(buffer[i]);
		kine->setKinematicTarget(settings->pose);
		//printf("p(%f, %f, %f)\n", settings->pose.p.x, settings->pose.p.y, settings->pose.p.z);

		physx::PxVec3 velocity = kine->getLinearVelocity();
		// Pick only the speed if above threshold
		if (velocity != physx::PxVec3(0) && velocity.magnitudeSquared() > 0.00005) {
			settings->linearVelocity = velocity * factor;
		}
		else {
			settings->linearVelocity = physx::PxVec3(physx::PxIdentity);
		}
		//physx::PxVec3 localVelocity = physx::PxRigidBodyExt::getLocalVelocityAtLocalPos(*kine, physx::PxVec3(100, 100, 100));
		//printf("l(%f); v(%f, %f, %f); lv(%f, %f, %f); f(%f)\n", settings->linearVelocity.magnitudeSquared(), settings->linearVelocity.x, settings->linearVelocity.y, settings->linearVelocity.z, localVelocity.x, localVelocity.y, localVelocity.z, factor);

		physx::PxVec3 angularVelocity = kine->getAngularVelocity();
		// Pick only the speed if above threshold
		//if (velocity != physx::PxVec3(0) && angulerVelocity.magnitudeSquared() > 0.00005) {
		settings->angularVelocity = angularVelocity * factor;
		//}
		//else {
		//	settings->angularVelocity = physx::PxVec3(physx::PxIdentity);
		//}
	}

	delete[] buffer;
}

/// \brief Apply physX entity transform to 3dgs entity
void PhysX3Util::TransformRigidEntity(physx::PxRigidActor *actor)
{
	if (NULL == actor || NULL == actor->userData)
	{
		return;
	}
	
	KineSettings *settings = (KineSettings *)actor->userData;

	ENTITY *entity = (ENTITY *)ptr_for_handle(settings->entityId);

	physx::PxTransform transform(actor->getGlobalPose());
	
	// Transform entity rotation to quaternion
	PxQuatToAng(transform.q, (ANGLE*)&(entity->pan));

	// Scale entity position
	PxVec3ToVec(transform.p, (VECTOR*)&(entity->x));

	// Save back settings
	/*if (settings->pose.p == transform.p)
	{
		settings->linearVelocity = physx::PxVec3(physx::PxIdentity);
	}*/
	settings->pose = transform;
}
