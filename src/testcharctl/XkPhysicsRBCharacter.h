#ifndef XK_PHYSICS_RB_CHARACTER_H
#define XK_PHYSICS_RB_CHARACTER_H

#include <string>
#include <Ogre.h>
#include "XkStepListener.h"

#include <Common/Base/hkBase.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBody.h>
#include <Physics/Utilities/CharacterControl/CharacterRigidBody/hkpCharacterRigidBodyListener.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpCharacterContext.h>

namespace Xk
{
    
class PhysicsRBCharacter : public StepListener
{
public:
    PhysicsRBCharacter(Ogre::SceneManager* psm, std::string strName, hkpCharacterRigidBodyCinfo& info);

public:
    bool step(float fTime);

    void applyMovement( 
        const float walkSpeed, 
        const float sideStepSpeed,
        const Ogre::Vector3& forwardDirection,
        const bool bJump );

    Ogre::SceneNode* getNode();

    unsigned int GetState();

private:
    Ogre::SceneManager* m_psm;
    std::string m_strName;
    Ogre::SceneNode* m_pNode;

    hkpCharacterRigidBody* m_pCharRigidBody;

    hkpCharacterContext* m_pCharacterContext;

    float m_actualTime;
    float m_currentAngle;
};

}// namespace Xk

#endif