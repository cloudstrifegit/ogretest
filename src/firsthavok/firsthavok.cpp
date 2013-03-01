#include "XkPhysicsWorld.h"
#include "XkUtil.h"
#include <stdio.h>
#include "XkPhysicsBody.h"
#include "XkOgreApplication.h"
#include "XkOgreFrameListener.h"

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

class MyFrameListener : public XkOgreFrameListener
{
public:
    MyFrameListener(RenderWindow* win, Camera* cam) :
      XkOgreFrameListener(win, cam)
    {

    }

    bool frameStarted(const FrameEvent& evt)
    {
        Xk::world::instance().step(evt.timeSinceLastFrame);

        Ogre::Vector3 dir = mCamera->getDirection();
        printf("dir( %f, %f, %f )\n", dir.x, dir.y, dir.z );

        return true;
    }
};

class MyApplication : public XkOgreApplication
{
public:
    void createFrameListener(void)
    {
        m_pFrameListener = new MyFrameListener(m_pWindow, m_pCamera);
        m_pFrameListener->showDebugOverlay(true);
        m_pRoot->addFrameListener(m_pFrameListener);
    }

    void createCamera(void)
    {
        XkOgreApplication::createCamera();

        m_pCamera->setPosition(20, 20, 20);
        m_pCamera->lookAt(0, 0, 0);
    }

    void createScene(void)
    {
        m_pSceneMgr->createLight()->setPosition(100, 100, 100);

        hkVector4 boxSize(0.5f, 0.5f, 0.5f);
        hkpBoxShape* boxShape = new hkpBoxShape(boxSize, 0);

        hkpRigidBodyCinfo info;
        info.m_mass = 10.0f;
        hkMassProperties massPro;
        hkVector4 halfExt(0.5f, 0.5f, 0.5f);
        hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExt, info.m_mass, massPro);

        info.m_mass = massPro.m_mass;
        info.m_centerOfMass = massPro.m_centerOfMass;
        info.m_inertiaTensor = massPro.m_inertiaTensor;
        info.m_solverDeactivation = info.SOLVER_DEACTIVATION_MEDIUM;
        info.m_shape = boxShape;
        info.m_restitution = 0.0f;

        info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
        hkVector4 pos(0, 10.0f, 0);
        info.m_position = pos;
        
        Xk::PhysicsBody* pBody = new Xk::PhysicsBody(m_pSceneMgr, "box1", info);
        Xk::world::instance().addEntity("box1", pBody);

        info.m_position = hkVector4(0.2f, 11.0f, 0);
        Xk::PhysicsBody* pBody2 = new Xk::PhysicsBody(m_pSceneMgr, "box2", info);
        Xk::world::instance().addEntity("box2", pBody2);
        
        Ogre::ManualObject* pDbgEntity = Xk::BuildDebugBox(m_pSceneMgr, 0.5f, "box1_debug", "Ogre/Skin");
        pBody->getNode()->attachObject(pDbgEntity);
        Ogre::Entity* pEntity = m_pSceneMgr->createEntity("box1", "1x1x1box.mesh");
        pBody->getNode()->attachObject(pEntity);

        Ogre::ManualObject* pDbgEntity2 = Xk::BuildDebugBox(m_pSceneMgr, 0.5f, "box2_debug", "Ogre/Skin");
        pBody2->getNode()->attachObject(pDbgEntity2);
        Ogre::Entity* pEntity2 =m_pSceneMgr->createEntity("box2", "1x1x1box.mesh");
        pBody2->getNode()->attachObject(pEntity2);

        int nCount = 5;
        for(int i=0; i<nCount; i++) {
            for(int j=0; j<nCount; j++) {
                for(int k=0; k<nCount; k++) {
                    info.m_position = hkVector4(i, 50.0f + j, k);
                    std::string strBoxName = "box_" + Ogre::StringConverter::toString(i) + Ogre::StringConverter::toString(j) + Ogre::StringConverter::toString(k);
                    std::string strDbgBoxName = "box_debug_" + Ogre::StringConverter::toString(i) + Ogre::StringConverter::toString(j) + Ogre::StringConverter::toString(k);
                    Xk::PhysicsBody* pBox = new Xk::PhysicsBody(m_pSceneMgr, strBoxName, info);
                    Xk::world::instance().addEntity(strBoxName, pBox);

                    //pBox->getNode()->scale(0.2, 0.2, 0.2);
                    Ogre::Entity* pBoxEntity = m_pSceneMgr->createEntity(strBoxName, "1x1x1box.mesh");
                    pBox->getNode()->attachObject(pBoxEntity);
                    Ogre::ManualObject* pDbgEntity = Xk::BuildDebugBox(m_pSceneMgr, 0.5f, strDbgBoxName, "Ogre/Skin");
                    pBox->getNode()->attachObject(pDbgEntity);
                }
            }
        }

        //创建ground
        hkVector4 groundSize(50.0f, 0.5f, 50.0f);
        hkpConvexShape* shape = new hkpBoxShape(groundSize, 0);

        hkpRigidBodyCinfo ci;
        ci.m_shape = shape;
        ci.m_motionType = hkpMotion::MOTION_FIXED;
        ci.m_position = hkVector4(0.0f, -0.5f, 0.0f);
        ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
        Xk::PhysicsBody* pGround = new Xk::PhysicsBody(m_pSceneMgr, "ground", ci);
        pGround->getNode()->scale(100.0f, 1.0f, 100.0f);
        Xk::world::instance().addEntity("ground", pGround);

        Ogre::ManualObject* pGroundEntityBox = Xk::BuildDebugBox(m_pSceneMgr, 0.5f, "grounddebug", "Ogre/Skin");
        Ogre::Entity* pGroundEntity = m_pSceneMgr->createEntity("ground", "1x1x1box.mesh");
        pGround->getNode()->attachObject(pGroundEntityBox);
        pGround->getNode()->attachObject(pGroundEntity);

        //创建胶囊
        hkVector4 top(0, 0.5f, 0);
        hkVector4 bottom(0, -0.5f, 0);

        hkpShape* capsuleShape = new hkpCapsuleShape(top, bottom, 0.5f);
        hkpRigidBodyCinfo capInfo;
        capInfo.m_shape = capsuleShape;
        capInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;

        capInfo.m_mass = 10.0f;
        hkMassProperties massCap;
        hkpInertiaTensorComputer::setShapeVolumeMassProperties(capInfo.m_shape, capInfo.m_mass, capInfo);
        
        capInfo.m_position = hkVector4(0, 2.0f, 0);

        Xk::PhysicsBody* pCapsule = new Xk::PhysicsBody(m_pSceneMgr, "capsule", capInfo);
        Xk::world::instance().addEntity("capsule", pCapsule);
        Ogre::Entity* pCapsuleEntity = m_pSceneMgr->createEntity("capsule", "capsule.mesh");
        pCapsule->getNode()->attachObject(pCapsuleEntity);

        hkpShape* sphereShape = new hkpSphereShape(0.5f);
        hkpRigidBodyCinfo sphereInfo;
        sphereInfo.m_shape = sphereShape;
        sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
        sphereInfo.m_mass = 10.0f;
        hkpInertiaTensorComputer::setShapeVolumeMassProperties(sphereInfo.m_shape, sphereInfo.m_mass, sphereInfo);
        
        sphereInfo.m_position = hkVector4(0, 1.0f, 0);
        Xk::PhysicsBody* pSphere = new Xk::PhysicsBody(m_pSceneMgr, "sphere", sphereInfo);
        Xk::world::instance().addEntity("sphere", pSphere);
        Ogre::Entity* pSphereEntity = m_pSceneMgr->createEntity("sphere", "sphere.mesh");
        pSphere->getNode()->attachObject(pSphereEntity);
    }
};

int main(int argc, char** argv)
{
    Xk::world::instance().initialize();

    MyApplication app;

    try{
        app.go();
    }
    catch(Ogre::Exception& e)
    {
        
    }

    return 0;
}