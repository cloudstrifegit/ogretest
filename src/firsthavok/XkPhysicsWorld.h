#ifndef XK_PHYSICS_WORLD_H
#define XK_PHYSICS_WORLD_H

#include "XkSingleton.h"

class hkMemoryRouter;
class hkJobThreadPool;
class hkJobQueue;
class hkpWorld;

namespace Xk
{

class XkPhysicsWorld
{
public:
    XkPhysicsWorld();
    ~XkPhysicsWorld();

    static void errorReport(const char* msg, void* userArgGivenToInit);

public:
    bool initialize();
    void uninitialize();
    bool step(float fTime);

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
};

typedef Singleton<XkPhysicsWorld> PhysicsWorld;

}//namespace Xk

#endif