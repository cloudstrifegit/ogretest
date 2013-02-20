#include "XkPhysicsWorld.h"

// Math and base include
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>

// Dynamics includes
#include <Physics/Collide/hkpCollide.h>										
#include <Physics/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>	
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>				
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>					

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics/Dynamics/World/hkpWorld.h>								
#include <Physics/Dynamics/Entity/hkpRigidBody.h>							
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>	

#include <stdio.h>

// Keycode
#include <Common/Base/keycode.cxx>

#if !defined HK_FEATURE_PRODUCT_PHYSICS
#error Physics is needed to build this demo. It is included in the common package for reference only.
#endif

// This excludes libraries that are not going to be linked
// from the project configuration, even if the keycodes are
// present

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_NEW_PHYSICS
#define HK_FEATURE_REFLECTION_PHYSICS
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches 
#define HK_EXCLUDE_LIBRARY_hkGeometryUtilities
#include <Common/Base/Config/hkProductFeatures.cxx>

namespace Xk
{

PhysicsWorld::PhysicsWorld() :
    m_accumulator(0.0f), m_stepSize(1.0f / 60.0f)
{

}

PhysicsWorld::~PhysicsWorld()
{

}

void PhysicsWorld::errorReport(const char* msg, void* userArgGivenToInit)
{
	printf("%s", msg);
}

bool PhysicsWorld::initialize()
{
	m_memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator,
		hkMemorySystem::FrameInfo(5000*1024));

	hkBaseSystem::init(m_memoryRouter, errorReport);

	hkHardwareInfo hwInfo;
	hkGetHardwareInfo(hwInfo);

	m_nThreadUsed = hwInfo.m_numThreads;

	hkCpuJobThreadPoolCinfo threadPoolCinfo;
	threadPoolCinfo.m_numThreads = m_nThreadUsed - 1;

	threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
	m_threadPool = new hkCpuJobThreadPool(threadPoolCinfo);

	hkJobQueueCinfo info;
	info.m_jobQueueHwSetup.m_numCpuThreads = m_nThreadUsed;
	m_jobQueue = new hkJobQueue(info);

	hkMonitorStream::getInstance().resize(200000);
	
	return createPhysicsWorld();
}

void PhysicsWorld::uninitialize()
{
	{
		m_physicsWorld->markForWrite();
		m_physicsWorld->removeReference();
        m_physicsWorld->unmarkForWrite();
	}

#ifdef XK_HAVOK_USE_VDB
	m_vdb->removeReference();
	m_physicsContext->removeReference();
#endif

	delete m_jobQueue;

	m_threadPool->removeReference();
	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}

bool PhysicsWorld::step(float fTime)
{
	m_accumulator += fTime;
	if(m_accumulator < m_stepSize) return false;

	m_accumulator -= m_stepSize;

	m_physicsWorld->stepMultithreaded(m_jobQueue,
		m_threadPool, m_stepSize);

#ifdef XK_HAVOK_USE_VDB
	m_physicsContext->syncTimers(m_threadPool);
	m_vdb->step();
#endif

	hkMonitorStream::getInstance().reset();
	m_threadPool->clearTimerData();

    for(MAP_ENTITY::iterator it = m_mapEntity.begin(); it != m_mapEntity.end(); it ++) {
        it->second->step(m_stepSize);
    }

	return true;
}

bool PhysicsWorld::addEntity(std::string strName, StepListener* pEntity)
{
    MAP_ENTITY::iterator it = m_mapEntity.find(strName);
    if(it != m_mapEntity.end()) return false;

    m_mapEntity.insert(MAP_ENTITY::value_type(strName, pEntity));

    m_physicsWorld->markForWrite();
    m_physicsWorld->addEntity(pEntity->getRigidBody());
    m_physicsWorld->unmarkForWrite();

    return true;
}

bool PhysicsWorld::createPhysicsWorld()
{
	hkpWorldCinfo worldInfo;
	worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
	worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;
	m_physicsWorld = new hkpWorld(worldInfo);

#ifdef XK_HAVOK_USE_VDB
	//如果使用VDB
	m_physicsWorld->m_wantDeactivation = false;
#endif
	m_physicsWorld->markForWrite();
	hkpAgentRegisterUtil::registerAllAgents(m_physicsWorld->getCollisionDispatcher());
	m_physicsWorld->registerWithJobQueue(m_jobQueue);
	
#ifdef XK_HAVOK_USE_VDB
	hkArray<hkProcessContext*>	processContexts;
	m_physicsContext = new hkpPhysicsContext();
	hkpPhysicsContext::registerAllPhysicsProcesses();
	m_physicsContext->addWorld(m_physicsWorld);
	processContexts.pushBack(m_physicsContext);
#endif
	m_physicsWorld->unmarkForWrite();

#ifdef XK_HAVOK_USE_VDB
	m_vdb = new hkVisualDebugger(processContexts);
	m_vdb->serve();
#endif

	return true;
} 

}//namespace Xk