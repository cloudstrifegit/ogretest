#include "XkOgreApplication.h"
#include "XkOgreFrameListener.h"
#include "XkPhysicsBody.h"
#include "XkPhysicsWorld.h"

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

#include <Physics/Dynamics/Constraint/Bilateral/Ragdoll/hkpRagdollConstraintData.h>

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

        Xk::PhysicsBody* pMoveBody;
        Xk::PhysicsBody* pFixedBody;

        hkVector4 pivot( 0, 0, 0 );
        hkVector4 halfSize( 0.5f, 0.5f, 0.5f );

        hkpBoxShape* boxShape;
        {
            boxShape = new hkpBoxShape( halfSize, 0 );
        }

        //Create a fixed body
        {
            hkpRigidBodyCinfo info;
            info.m_position.set( -1.0f, 0.0f, 0.0f );
            info.m_shape = boxShape;
            info.m_motionType = hkpMotion::MOTION_FIXED;
            pFixedBody = new Xk::PhysicsBody( m_pSceneMgr,
                "fixedBody",
                info );
            Xk::world::instance().addEntity( "fixedBody", pFixedBody );
            Ogre::Entity* pFixedEntity = m_pSceneMgr->createEntity( "1x1x1box.mesh" );
            pFixedBody->getNode()->attachObject( pFixedEntity );
        }

        {
            hkpRigidBodyCinfo info;
            info.m_position.set( 2.0f, 0.0f, 0.0f );
            info.m_shape = boxShape;

            hkMassProperties massProperties;
            info.m_mass = 10.0f;
            hkpInertiaTensorComputer::computeBoxVolumeMassProperties( halfSize, info.m_mass, massProperties );
            info.m_inertiaTensor = massProperties.m_inertiaTensor;
            info.m_centerOfMass = massProperties.m_centerOfMass;
            info.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
            pMoveBody = new Xk::PhysicsBody( m_pSceneMgr,
                "movebody",
                info );
            Xk::world::instance().addEntity( "movebody", pMoveBody );

            Ogre::Entity* pMoveEntity = m_pSceneMgr->createEntity( "1x1x1box.mesh" );
            pMoveBody->getNode()->attachObject( pMoveEntity );
        }

        boxShape->removeReference();
        boxShape = HK_NULL;

            //
	    // CREATE RAGDOLL CONSTRAINT
	    // 
	    {
		    hkReal planeMin =  HK_REAL_PI * -0.2f;
		    hkReal planeMax =  HK_REAL_PI * 0.1f;
		    hkReal twistMin =  HK_REAL_PI * -0.1f;
		    hkReal twistMax =  HK_REAL_PI *  0.4f;
		    hkReal coneMin  =  HK_REAL_PI * 0.3f;

		    hkpRagdollConstraintData* rdc = new hkpRagdollConstraintData();

		    rdc->setConeAngularLimit(coneMin);
		    rdc->setPlaneMinAngularLimit(planeMin);
		    rdc->setPlaneMaxAngularLimit(planeMax);
		    rdc->setTwistMinAngularLimit(twistMin);
		    rdc->setTwistMaxAngularLimit(twistMax);

		    hkVector4 twistAxis(1.0f, 0.0f, 0.0f);
		    hkVector4 planeAxis(0.0f, 1.0f, 0.0f);
		    pivot.set(0.0f, 0.0f, 0.0f);
            rdc->setInWorldSpace(pMoveBody->getRigidBody()->getTransform(), 
                pFixedBody->getRigidBody()->getTransform(), pivot, twistAxis, planeAxis);
		
		    //
		    //	Create and add the constraint
		    //
		    {
                hkpConstraintInstance* constraint = new hkpConstraintInstance(pMoveBody->getRigidBody(), pFixedBody->getRigidBody(), rdc );
                Xk::world::instance().getPhysicsWorld()->addConstraint(constraint);
			    constraint->removeReference();
		    }		

		    rdc->removeReference();
	    }
    }
};

int main(int argc, char** argv)
{
    Xk::world::instance().initialize();

    try{
	    MyApplication app;
        app.go();
    }
    catch(Ogre::Exception& e)
    {
        MessageBox(NULL, e.getDescription().c_str(), "Ogre::Exception", MB_OK);
    }

    return 0;
}