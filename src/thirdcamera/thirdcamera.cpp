#include "XkOgreApplication.h"
#include "XkOgreFrameListener.h"
#include "ThirdPersonCamera.h"

//Global
ThirdPersonCamera* g_pThirdCamera;

class MyFrameListener : public XkOgreFrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
    MyFrameListener( RenderWindow* win, Camera* cam ) :
      XkOgreFrameListener( win, cam, true, true )
    {
        mKeyboard->setEventCallback( this );
        mMouse->setEventCallback( this );
    }

    bool processUnbufferedKeyInput(const FrameEvent& evt)
    {
        if( mKeyboard->isKeyDown(OIS::KC_ESCAPE) || mKeyboard->isKeyDown(OIS::KC_Q) )
			return false;
        
        return true;
    }

    bool keyPressed( const OIS::KeyEvent &arg )
    {
        g_pThirdCamera->injectKeyDown( arg );
        return true;
    }

    bool keyReleased( const OIS::KeyEvent &arg )
    {
        g_pThirdCamera->injectKeyUp( arg );
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
        g_pThirdCamera->updateBody( evt.timeSinceLastFrame );
        g_pThirdCamera->updateCamera( evt.timeSinceLastFrame );

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

    void createScene(void)
    {
        Ogre::Light* pLight = m_pSceneMgr->createLight();
        pLight->setPosition( 100.0f, 100.0f ,100.0f );

        Ogre::SceneNode* pHeadNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
        Ogre::Entity* pEntity = m_pSceneMgr->createEntity( "ogrehead", "ogrehead.mesh" );
        pHeadNode->attachObject( pEntity );
        pHeadNode->scale( 0.3, 0.3 ,0.3 );

        Ogre::Entity* pEntity2 = m_pSceneMgr->createEntity( "ogrehead2", "ogrehead.mesh" );
        Ogre::SceneNode* pNode2 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
        pNode2->attachObject( pEntity2 );
        pNode2->setPosition( 0, 0, 20 );
        pNode2->scale( 0.3, 0.3, 0.3 );

        g_pThirdCamera = new ThirdPersonCamera( m_pCamera, pHeadNode );
        g_pThirdCamera->setup();
    }

};

int main(int argc, char** argv)
{
    MyApplication app;

    try{
        app.go();
    }
    catch(Ogre::Exception& e)
    {
        
    }

    return 0;
}