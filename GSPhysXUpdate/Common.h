#pragma once

#define DegreeToRadian(fDegrees) (0.01745329251994329576923690768489f * fDegrees) // PI / 180
#define RadianToDegree(fRadian) (57.295779513082320876798154814105f * fRadian)	// 180 / PI

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