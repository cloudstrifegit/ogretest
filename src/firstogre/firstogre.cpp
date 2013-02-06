#include "XkOgreApplication.h"

class MyApplication : public XkOgreApplication
{
    void createScene()
    {
        m_pSceneMgr->createLight()->setPosition(100, 100, 100);

        Ogre::Entity* pEntity = m_pSceneMgr->createEntity("OgreHead", "ogrehead.mesh");
        Ogre::SceneNode* pNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
        pNode->attachObject(pEntity);
    }
};

int main()
{
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