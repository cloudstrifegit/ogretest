#include "XkPhysicsWorld.h"
#include "XkUtil.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    Xk::PhysicsWorld::instance().initialize();

    for(int i=0; i<10000; i++) {
        Xk::PhysicsWorld::instance().step(0.1f);
    }

    Xk::PhysicsWorld::instance().uninitialize();

    Ogre::Vector3 vec(1, 2, 3);
    hkVector4 v = Xk::tohkVector4(vec);

    getchar();

    return 0;
}