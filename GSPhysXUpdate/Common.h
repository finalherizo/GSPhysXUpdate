#pragma once

#define DEGTORADFACTOR 0.01745329251994329576923690768489f
#define RADTODEGFACTOR 57.295779513082320876798154814105f
#define CONV_DEGTORAD(fDegrees) (DEGTORADFACTOR * fDegrees) // PI / 180
#define CONV_RADTODEG(fRadian) (RADTODEGFACTOR * fRadian)	// 180 / PI

#define PX_RELEASE(sys) if (NULL != sys) {sys->release(); sys = NULL;}

#define PX_DELETE(sys) if (NULL != sys) {delete sys; sys = NULL;}

// Substepping mode
#define PX_TIMESTEP_FIXED 0
#define PX_TIMESTEP_VARIABLE 1

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <cstdio>
#include <adll.h>

#ifdef v
#undef v
#define av(a) (*ev->a)
#endif

#include <PxPhysicsAPI.h>
#include <extensions\PxDefaultAllocator.h>
#include <extensions\PxDefaultErrorCallback.h>
#include <extensions\PxDefaultSimulationFilterShader.h>
#include <extensions\PxDefaultCpuDispatcher.h>
#include <cooking\PxCooking.h>
#include <physxprofilesdk\PxProfileZoneManager.h>
#include <extensions\PxExtensionsAPI.h>
#include <foundation\PxSimpleTypes.h>