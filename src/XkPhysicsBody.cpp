#include "XkPhysicsBody.h"
#include "XkUtil.h"

namespace Xk
{

PhysicsBody::PhysicsBody(Ogre::SceneManager* psm, std::string strName, hkpRigidBodyCinfo& info) 
    : m_psm(psm), m_strName(strName)
{
    m_rigidBody = new hkpRigidBody(info);
    m_pNode = m_psm->getRootSceneNode()->createChildSceneNode(m_strName);
}

Ogre::SceneNode* PhysicsBody::getNode()
{
    return m_pNode;
}

bool PhysicsBody::step(float fTime)
{
    hkVector4 pos = m_rigidBody->getPosition();
    hkQuaternion quat = m_rigidBody->getRotation();

    Ogre::Vector3 v3 = Xk::toVector3(pos);
    Ogre::Quaternion q = Xk::toQuaternion(quat);

    m_pNode->setPosition(v3);
    m_pNode->setOrientation(q);

    return true;
}

}//namespace Xk