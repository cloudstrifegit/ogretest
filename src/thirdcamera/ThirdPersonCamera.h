#ifndef _OGRETEST_THIRD_PERSON_CAMERA_H_
#define _OGRETEST_THIRD_PERSON_CAMERA_H_

#include <Ogre.h>

#define CAM_HEIGHT 2
#define TURN_SPEED 500.0f
#define RUN_SPEED 30
#define CAMERA_MIN_DIST 8
#define CAMERA_MAX_DIST 40

class ThirdPersonCamera
{
public:
    ThirdPersonCamera( Ogre::Camera* pCam, Ogre::SceneNode* pNode ) : m_pCamera( pCam ), m_pBodyNode( pNode )
    {

    }

    bool setup()
    {
        m_pCameraPivot = m_pCamera->getSceneManager()->getRootSceneNode()->createChildSceneNode();
        m_pCameraGoal = m_pCameraPivot->createChildSceneNode( Ogre::Vector3( 0, 0, 15) );
        m_pCameraNode = m_pCamera->getSceneManager()->getRootSceneNode()->createChildSceneNode();
        m_pCameraNode->setPosition( m_pCameraPivot->getPosition() + m_pCameraGoal->getPosition() );

        m_pCameraPivot->setFixedYawAxis( true );
        m_pCameraGoal->setFixedYawAxis( true );
        m_pCameraNode->setFixedYawAxis( true );

        m_pCamera->setNearClipDistance( 0.1 );
        m_pCamera->setFarClipDistance( 100 );
        m_pCameraNode->attachObject( m_pCamera );

        m_KeyDirection = Ogre::Vector3::ZERO;
        m_PivotPitch = 0;

        return true;
    }

    void updateBody( Ogre::Real deltaTime )
    {
        m_GoalDirection = Ogre::Vector3::ZERO;

        if( m_KeyDirection != Ogre::Vector3::ZERO ) {
            m_GoalDirection += m_KeyDirection.z * m_pCameraNode->getOrientation().zAxis();
            m_GoalDirection += m_KeyDirection.x * m_pCameraNode->getOrientation().xAxis();
            m_GoalDirection.y = 0;
            m_GoalDirection.normalise();

            Ogre::Quaternion toGoal = m_pBodyNode->getOrientation().zAxis().getRotationTo( m_GoalDirection );

            Ogre::Real yawToGoal = toGoal.getYaw().valueDegrees();
            Ogre::Real yawAtSpeed = yawToGoal / Math::Abs( yawToGoal ) * deltaTime * TURN_SPEED;
            if( yawToGoal < 0 ) yawToGoal = std::min<Ogre::Real>( 0, std::max<Ogre::Real>( yawToGoal, yawAtSpeed ) );
            else if ( yawToGoal > 0) yawToGoal = std::max<Ogre::Real>( 0, std::min<Ogre::Real>( yawToGoal, yawAtSpeed ) );

            m_pBodyNode->yaw( Ogre::Degree( yawToGoal ) );

            m_pBodyNode->translate( 0, 0, deltaTime * RUN_SPEED, Ogre::Node::TS_LOCAL );
        }
    }

    void updateCamera( Ogre::Real deltaTime )
    {
        m_pCameraPivot->setPosition( m_pBodyNode->getPosition() + Ogre::Vector3::UNIT_Y * CAM_HEIGHT );

        Ogre::Vector3 goalOffset = m_pCameraGoal->_getDerivedPosition() - m_pCameraNode->getPosition();
        m_pCameraNode->translate( goalOffset * deltaTime * 9.0f );
        m_pCameraNode->lookAt( m_pCameraPivot->_getDerivedPosition(), Ogre::Node::TS_WORLD );
    }

    void updateCameraGoal( Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom )
    {
        m_pCameraPivot->yaw( Ogre::Degree( deltaYaw ), Ogre::Node::TS_WORLD );
        if( !(m_PivotPitch + deltaPitch > 25 && deltaPitch > 0 ) &&
            !(m_PivotPitch + deltaPitch < -60 && deltaPitch < 0 ) )
        {
            m_pCameraPivot->pitch( Ogre::Degree( deltaPitch * 2 ), Ogre::Node::TS_LOCAL );
            m_PivotPitch += deltaPitch;
        }

        Ogre::Real dist = m_pCameraGoal->_getDerivedPosition().distance( m_pCameraPivot->_getDerivedPosition() );
        Ogre::Real distChange = deltaZoom * dist;

        if( !(dist + distChange < CAMERA_MIN_DIST && distChange < 0 ) &&
            !(dist + distChange > CAMERA_MAX_DIST && distChange > 0 ) )
        {
            m_pCameraGoal->translate( 0, 0, distChange * 2, Ogre::Node::TS_LOCAL );
        }
    }

    void injectKeyDown( const OIS::KeyEvent& evt )
    {
        if(evt.key == OIS::KC_W) m_KeyDirection.z = -1;
        else if ( evt.key == OIS::KC_A ) m_KeyDirection.x = -1;
        else if ( evt.key == OIS::KC_S ) m_KeyDirection.z = 1;
        else if ( evt.key == OIS::KC_D ) m_KeyDirection.x = 1;
    }

    void injectKeyUp( const OIS::KeyEvent& evt )
    {
        if( evt.key == OIS::KC_W && m_KeyDirection.z == -1 )
            m_KeyDirection.z = 0;
        else if ( evt.key == OIS::KC_A && m_KeyDirection.x == -1 )
            m_KeyDirection.x = 0;
        else if ( evt.key == OIS::KC_S && m_KeyDirection.z == 1 )
            m_KeyDirection.z = 0;
        else if ( evt.key == OIS::KC_D && m_KeyDirection.x == 1 )
            m_KeyDirection.x = 0;
    }

    void injectMouseMove( const OIS::MouseEvent& evt)
    {
        updateCameraGoal( -0.05f * evt.state.X.rel, -0.05f * evt.state.Y.rel, -0.0005f * evt.state.Z.rel );
    }

    void injectMouseDown( const OIS::MouseEvent& evt, OIS::MouseButtonID id)
    {

    }

private:
    Ogre::Camera* m_pCamera;
    Ogre::SceneNode* m_pBodyNode;
    
    Ogre::SceneNode* m_pCameraPivot;
    Ogre::SceneNode* m_pCameraGoal;
    Ogre::SceneNode* m_pCameraNode;

    Ogre::Real m_PivotPitch;
    Ogre::Vector3 m_GoalDirection;
    Ogre::Vector3 m_KeyDirection;
};

#endif