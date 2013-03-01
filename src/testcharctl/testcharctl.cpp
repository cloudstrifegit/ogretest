#include "XkOgreApplication.h"
#include "XkOgreFrameListener.h"
#include "ThirdPersonCamera.h"
#include "XkUtil.h"
#include "XkPhysicsBody.h"
#include "XkPhysicsWorld.h"
#include "XkPhysicsRBCharacter.h"

#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

//Global
ThirdPersonCamera* g_pThirdCamera;
Xk::PhysicsRBCharacter* g_pRBChar;

class MyFrameListener : public XkOgreFrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
    MyFrameListener( RenderWindow* win, Camera* cam ) :
      XkOgreFrameListener( win, cam, true, true )
    {
        mKeyboard->setEventCallback( this );
        mMouse->setEventCallback( this );
        m_bJump = false;
        m_timer = 0.0f;
    }
    
    bool keyPressed( const OIS::KeyEvent &arg )
    {
        //g_pThirdCamera->injectKeyDown( arg );
        if( arg.key == OIS::KC_SPACE ) {
            if( g_pRBChar->GetState() == HK_CHARACTER_ON_GROUND ) {
                m_bJump = true;
            }else{
                m_bJump = false;
            }
        }

        return true;
    }

    bool keyReleased( const OIS::KeyEvent &arg )
    {
        //g_pThirdCamera->injectKeyUp( arg );
        if( arg.key == OIS::KC_SPACE ) {
            m_bJump = false;
        }

        return true;
    }

    bool mouseMoved( const OIS::MouseEvent& evt )
    {
        g_pThirdCamera->injectMouseMove( evt );
        return true;
    }

    bool mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id )
    {
        return true;
    }

    bool mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id )
    {
        return true;
    }

    bool frameStarted( const FrameEvent& evt )
    {
        m_timer += evt.timeSinceLastFrame;

        g_pThirdCamera->updateBody( evt.timeSinceLastFrame );
        g_pThirdCamera->updateCamera( evt.timeSinceLastFrame );

        Xk::world::instance().step(evt.timeSinceLastFrame);

        float posX = 0.0f;
        float posY = 0.0f;

        if(mKeyboard->isKeyDown(OIS::KC_A))
		{
			posY=1.0f;
		}
		if(mKeyboard->isKeyDown(OIS::KC_D))
		{
			posY=-1.0f;
		}
		if(mKeyboard->isKeyDown(OIS::KC_W))
		{
			posX = -1.0f;
		}
		if(mKeyboard->isKeyDown(OIS::KC_S))
		{
			posX = 1.0f;
		}

        if(mKeyboard->isKeyDown(OIS::KC_Q)){
			return false;
		}

        Ogre::Vector3 dir = mCamera->getDerivedDirection();

        Ogre::Vector3 realDir( dir.x, 0, dir.z );

        Xk::world::instance().lock();

        g_pRBChar->applyMovement( posX, posY, realDir, m_bJump );
        
        Xk::world::instance().unlock();

        return true;
    }

private:
    bool m_bJump;
    float m_timer;
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

    void createScene(void)
    {
        Ogre::Light* pLight = m_pSceneMgr->createLight();
        pLight->setPosition( 100.0f, 100.0f ,100.0f );

        //创建地面
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
        Ogre::Entity* pGroundEntity = m_pSceneMgr->createEntity("ground", "1x1x1box.mesh");
        pGround->getNode()->attachObject( pGroundEntity );

        //创建Character
        hkVector4 top( 0, 0.5f, 0 );
        hkVector4 bottom( 0, -0.5f, 0 );
        hkpShape* standCapsule = new hkpCapsuleShape( top, bottom, 0.5f );

        hkpCharacterRigidBodyCinfo info;
        info.m_mass = 10.0f;
        info.m_shape = standCapsule;
        
        info.m_maxForce = 2000.0f;
        info.m_up = hkVector4( 0, 1, 0 );
        info.m_position.set( 10.0f, 80.0f, 6.0f );
        info.m_maxSlope = HK_REAL_PI/2.0f;

        g_pRBChar = new Xk::PhysicsRBCharacter( m_pSceneMgr, "character", info );
        Xk::world::instance().addEntity( "character", g_pRBChar );

        standCapsule->removeReference();

        Ogre::Entity* pCapsule = m_pSceneMgr->createEntity( "character", "capsule.mesh" );
        g_pRBChar->getNode()->attachObject( pCapsule );
        
        Ogre::ManualObject* pDebugAxis = Xk::BuildDebugAxis( m_pSceneMgr, 3.0f, "debugaxis" );
        g_pRBChar->getNode()->attachObject( pDebugAxis );

        hkVector4 boxSize(0.5f, 0.5f, 0.5f);
        hkpBoxShape* boxShape = new hkpBoxShape(boxSize, 0);

        hkpRigidBodyCinfo rbinfo;
        rbinfo.m_mass = 100.0f;
        hkMassProperties massPro;
        hkVector4 halfExt(0.5f, 0.5f, 0.5f);
        hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfExt, info.m_mass, massPro);

        rbinfo.m_mass = massPro.m_mass;
        rbinfo.m_centerOfMass = massPro.m_centerOfMass;
        rbinfo.m_inertiaTensor = massPro.m_inertiaTensor;
        rbinfo.m_solverDeactivation = rbinfo.SOLVER_DEACTIVATION_MEDIUM;
        rbinfo.m_shape = boxShape;
        rbinfo.m_restitution = 0.0f;

        rbinfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
        hkVector4 pos(0, 10.0f, 0);
        rbinfo.m_position = pos;
        
        Xk::PhysicsBody* pBody = new Xk::PhysicsBody(m_pSceneMgr, "box1", rbinfo);
        Xk::world::instance().addEntity("box1", pBody);

        Ogre::Entity* pBoxEntity = m_pSceneMgr->createEntity( "box1", "1x1x1box.mesh" );
        pBody->getNode()->attachObject( pBoxEntity );

        g_pThirdCamera = new ThirdPersonCamera( m_pCamera, g_pRBChar->getNode() );
        g_pThirdCamera->setup();

        //创建一堆box
        int nCount = 5;
        for(int i=0; i<nCount; i++) {
            for(int j=0; j<nCount; j++) {
                for(int k=0; k<nCount; k++) {
                    rbinfo.m_position = hkVector4(i, 50.0f + j, k);
                    std::string strBoxName = "box_" + Ogre::StringConverter::toString(i) + Ogre::StringConverter::toString(j) + Ogre::StringConverter::toString(k);
                    std::string strDbgBoxName = "box_debug_" + Ogre::StringConverter::toString(i) + Ogre::StringConverter::toString(j) + Ogre::StringConverter::toString(k);
                    Xk::PhysicsBody* pBox = new Xk::PhysicsBody(m_pSceneMgr, strBoxName, rbinfo);
                    Xk::world::instance().addEntity(strBoxName, pBox);

                    //pBox->getNode()->scale(0.2, 0.2, 0.2);
                    Ogre::Entity* pBoxEntity = m_pSceneMgr->createEntity(strBoxName, "1x1x1box.mesh");
                    pBox->getNode()->attachObject(pBoxEntity);
                    Ogre::ManualObject* pDbgEntity = Xk::BuildDebugBox(m_pSceneMgr, 0.5f, strDbgBoxName, "Ogre/Skin");
                    pBox->getNode()->attachObject(pDbgEntity);
                }
            }
        }
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