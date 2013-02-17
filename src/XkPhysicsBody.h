#ifndef XK_PHYSICS_BODY_H
#define XK_PHYSICS_BODY_H

#include <string>
#include <Ogre.h>
#include "XkStepListener.h"

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

namespace Xk
{

class PhysicsBody : public StepListener
{
public:
    PhysicsBody(Ogre::SceneManager* psm, std::string strName, hkpRigidBodyCinfo& info);

public:
    bool step(float fTime);

    Ogre::SceneNode* getNode();

private:
    Ogre::SceneManager* m_psm;
    std::string m_strName;
    //hkpRigidBody* m_rigidBody;
    Ogre::SceneNode* m_pNode;
};

}//namespace Xk

#endif