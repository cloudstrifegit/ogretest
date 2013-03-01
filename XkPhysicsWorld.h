#ifndef XK_PHYSICS_WORLD_H
#define XK_PHYSICS_WORLD_H

#include <string>
#include <map>
#include "XkSingleton.h"
#include "XkStepListener.h"

class hkMemoryRouter;
class hkJobThreadPool;
class hkJobQueue;
class hkpWorld;

namespace Xk
{

class PhysicsWorld
{
public:
    typedef std::map<std::string, StepListener*> MAP_ENTITY;

    PhysicsWorld();
    ~PhysicsWorld();

    static void errorReport(const char* msg, void* userArgGivenToInit);

public:
    bool initialize();
    void uninitialize();
    bool step(float fTime);

    bool addEntity(std::string strName, StepListener* pEntity);

    void lock();
    void unlock();

private:
    bool createPhysicsWorld();

private:
    hkMemoryRouter*		m_memoryRouter;
	hkJobThreadPool*	m_threadPool;
	hkJobQueue*			m_jobQueue;
	int					m_nThreadUsed;
	hkpWorld*			m_physicsWorld;

#ifdef XK_HAVOK_USE_VDB
	hkpPhysicsContext*			m_physicsContext;
	hkVisualDebugger*			m_vdb;
#endif

    float m_accumulator;
    float m_stepSize;

    MAP_ENTITY  m_mapEntity;
};

typedef Singleton<PhysicsWorld> world;

}//namespace Xk

#endif