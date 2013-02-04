#ifndef XK_OGRE_APPLICATION_H
#define XK_OGRE_APPLICATION_H

#include <OGRE/Ogre.h>
#include "XkOgreFrameListener.h"
#include <Overlay/OgreOverlaySystem.h>

class XkOgreApplication
{
public:
    XkOgreApplication();
    ~XkOgreApplication();

    virtual void go(void);

protected:
    Ogre::Root* m_pRoot;
    Ogre::Camera* m_pCamera;
    Ogre::SceneManager* m_pSceneMgr;
    XkOgreFrameListener* m_pFrameListener;
    Ogre::RenderWindow* m_pWindow;
    Ogre::String m_ResourcePath;

    Ogre::OverlaySystem* m_pOverlaySystem;

    virtual bool setup(void);
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0;
    virtual void destroyScene(void) { }
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);
};

#endif