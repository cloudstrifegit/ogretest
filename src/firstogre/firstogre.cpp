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
	MyApplication app;
    app.go();

    return 0;
}