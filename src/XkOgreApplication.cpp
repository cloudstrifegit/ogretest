#include "XkOgreApplication.h"

XkOgreApplication::XkOgreApplication()
{
    m_ResourcePath = "";
}

XkOgreApplication::~XkOgreApplication()
{
    if(m_pRoot) OGRE_DELETE m_pRoot;
}

void XkOgreApplication::go(void)
{
    if(!setup())
        return;

    m_pRoot->startRendering();

    destroyScene();
}

bool XkOgreApplication::setup(void)
{
    Ogre::String pluginsPath = "plugins.cfg";
    m_pRoot = OGRE_NEW Ogre::Root(pluginsPath,
        "Ogre.cfg",
        "Ogre.log");

    m_pOverlaySystem = OGRE_NEW Ogre::OverlaySystem();

    setupResources();

    bool carryOn = configure();
    if(!carryOn)
        return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    createResourceListener();
    loadResources();

    createScene();
    createFrameListener();

    return true;
}

bool XkOgreApplication::configure(void)
{
    if(m_pRoot->showConfigDialog()) {
        m_pWindow = m_pRoot->initialise(true);
        return true;
    }else{ 
        return false;
    }
}

void XkOgreApplication::chooseSceneManager(void)
{
    m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_GENERIC, "XkOgreSMInstance");
}

void XkOgreApplication::createCamera(void)
{
    m_pCamera = m_pSceneMgr->createCamera("PlayerCam");
    m_pCamera->setPosition(Ogre::Vector3(0, 0, 500));
    m_pCamera->lookAt(Ogre::Vector3(0, 0, -300));
    m_pCamera->setNearClipDistance(5);
}

void XkOgreApplication::createFrameListener(void)
{
    m_pFrameListener = new XkOgreFrameListener(m_pWindow, m_pCamera);
    m_pFrameListener->showDebugOverlay(true);
    m_pRoot->addFrameListener(m_pFrameListener);
}

void XkOgreApplication::createViewports(void)
{
    Ogre::Viewport* vp = m_pWindow->addViewport(m_pCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    m_pCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

void XkOgreApplication::setupResources(void)
{
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;

    while(seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for(i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

void XkOgreApplication::createResourceListener(void)
{

}

void XkOgreApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}