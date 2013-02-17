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
        //pNode2->attachObject(pNewEntity);
        pNode2->setPosition(0, 50, 0);

        Ogre::ManualObject* pObj = m_pSceneMgr->createManualObject();
        pObj->begin("Ogre/Skin", Ogre::RenderOperation::OT_LINE_LIST);
        float fHalfSize = 20.0f;

        //AB
        pObj->position(-fHalfSize, fHalfSize, -fHalfSize);
        pObj->position(fHalfSize, fHalfSize, -fHalfSize);
        //BC
        pObj->position(fHalfSize, fHalfSize, -fHalfSize);
        pObj->position(fHalfSize, fHalfSize, fHalfSize);
        //CD
        pObj->position(fHalfSize, fHalfSize, fHalfSize);
        pObj->position(-fHalfSize, fHalfSize, fHalfSize);
        //DA
        pObj->position(-fHalfSize, fHalfSize, fHalfSize);
        pObj->position(-fHalfSize, fHalfSize, -fHalfSize);

        //EF
        pObj->position(-fHalfSize, -fHalfSize, -fHalfSize);
        pObj->position(fHalfSize, -fHalfSize, -fHalfSize);
        //FG
        pObj->position(fHalfSize, -fHalfSize, -fHalfSize);
        pObj->position(fHalfSize, -fHalfSize, fHalfSize);
        //GH
        pObj->position(fHalfSize, -fHalfSize, fHalfSize);
        pObj->position(-fHalfSize, -fHalfSize, fHalfSize);
        //HE
        pObj->position(-fHalfSize, -fHalfSize, fHalfSize);
        pObj->position(-fHalfSize, -fHalfSize, -fHalfSize);

        //AE
        pObj->position(-fHalfSize, fHalfSize, -fHalfSize);
        pObj->position(-fHalfSize, -fHalfSize, -fHalfSize);
        //BF
        pObj->position(fHalfSize, fHalfSize, -fHalfSize);
        pObj->position(fHalfSize, -fHalfSize, -fHalfSize);
        //CG
        pObj->position(fHalfSize, fHalfSize, fHalfSize);
        pObj->position(fHalfSize, -fHalfSize, fHalfSize);
        //DH
        pObj->position(-fHalfSize, fHalfSize, fHalfSize);
        pObj->position(-fHalfSize, -fHalfSize, fHalfSize);

        /*
        pObj->triangle(0, 2, 1);
        pObj->triangle(0, 2, 3);
        pObj->triangle(3, 4, 0);
        pObj->triangle(3, 7, 4);
        pObj->triangle(4, 7, 6);
        pObj->triangle(4, 6, 5);
        pObj->triangle(5, 2, 1);
        pObj->triangle(5, 6, 2);
        pObj->triangle(0, 4, 1);
        pObj->triangle(5, 1, 4);
        pObj->triangle(3, 6, 7);
        pObj->triangle(3, 2, 6);
        */
        
        pObj->end();
        pNode2->attachObject(pObj);
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