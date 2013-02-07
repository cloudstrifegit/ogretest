#include "XkOgreApplication.h"

class MyApplication : public XkOgreApplication
{
    void createScene()
    {
        m_pSceneMgr->createLight()->setPosition(100, 100, 100);

        Ogre::Entity* pEntity = m_pSceneMgr->createEntity("OgreHead", "ogrehead.mesh");
        Ogre::Entity* pNewEntity = pEntity->clone("OgreHead2");

        Ogre::SceneNode* pNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
        pNode->attachObject(pEntity);
        
        Ogre::SceneNode* pNode2 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
        pNode2->attachObject(pNewEntity);
        pNode2->setPosition(0, 50, 0);
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