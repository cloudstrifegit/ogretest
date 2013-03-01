#include "XkPhysicsRBCharacter.h"
#include "XkUtil.h"

namespace Xk
{

PhysicsRBCharacter::PhysicsRBCharacter(Ogre::SceneManager* psm, std::string strName, hkpCharacterRigidBodyCinfo& info)
{
    m_pCharRigidBody = new hkpCharacterRigidBody( info );
    m_rigidBody = m_pCharRigidBody->getRigidBody();

    hkpCharacterRigidBodyListener* listener = new hkpCharacterRigidBodyListener;
    m_pCharRigidBody->setListener( listener );
    listener->removeReference();

    m_pNode = psm->getRootSceneNode()->createChildSceneNode( strName );

    hkpCharacterState* state;
    hkpCharacterStateManager* manager = new hkpCharacterStateManager();

    state = new hkpCharacterStateOnGround();
    manager->registerState( state, HK_CHARACTER_ON_GROUND );
    state->removeReference();

    state = new hkpCharacterStateInAir();
    manager->registerState( state, HK_CHARACTER_IN_AIR );
    state->removeReference();

    hkpCharacterStateJumping* stateJmp = new hkpCharacterStateJumping();
    stateJmp->setJumpHeight(20.0f);
    manager->registerState( stateJmp, HK_CHARACTER_JUMPING );
    stateJmp->removeReference();

    state = new hkpCharacterStateClimbing();
    manager->registerState( state, HK_CHARACTER_CLIMBING );
    state->removeReference();

    m_pCharacterContext = new hkpCharacterContext( manager, HK_CHARACTER_ON_GROUND );
    manager->removeReference();

    m_pCharacterContext->setCharacterType( hkpCharacterContext::HK_CHARACTER_RIGIDBODY );
    m_pCharacterContext->setFilterParameters( 0.9f, 15.0f, 400.0f );

    m_actualTime = 0.0f;
    m_currentAngle = 0.0f;
}

bool PhysicsRBCharacter::step(float fTime)
{
    m_actualTime += fTime;

    hkVector4 pos = m_pCharRigidBody->getRigidBody()->getPosition();
    hkQuaternion quat = m_pCharRigidBody->getRigidBody()->getRotation();

    Ogre::Vector3 v3 = Xk::toVector3(pos);
    Ogre::Quaternion q = Xk::toQuaternion(quat);

    m_pNode->setPosition(v3);
    m_pNode->setOrientation(q);

    return true;
}

void PhysicsRBCharacter::applyMovement(
        const float walkSpeed, 
        const float sideStepSpeed,
        const Ogre::Vector3& forwardDirection,
        const bool bJump )
{
    hkpCharacterInput input;
    hkpCharacterOutput output;

    input.m_inputLR = sideStepSpeed;
    input.m_inputUD = walkSpeed;

    input.m_wantJump = bJump;
    input.m_atLadder = false;

    input.m_up = hkVector4( 0, 1, 0);
    input.m_forward = tohkVector4( forwardDirection );

    float timestep = 1.0f / 60.0f;

    hkStepInfo stepInfo;
    stepInfo.m_deltaTime = timestep;
    stepInfo.m_invDeltaTime = 1.0f / timestep;
    stepInfo.m_endTime = m_actualTime;

    input.m_stepInfo = stepInfo;
    input.m_characterGravity.set( 0, -9, 0 );
    input.m_velocity = m_pCharRigidBody->getLinearVelocity();
    input.m_position = m_pCharRigidBody->getPosition();
    /*
    if(input.m_velocity(1) > 1) {
        printf("input.m_veclocity (%f, %f, %f)\n", input.m_velocity(0), input.m_velocity(1), input.m_velocity(2));
    }
    */

    m_pCharRigidBody->checkSupport( stepInfo, input.m_surfaceInfo );

    m_pCharacterContext->update( input, output );

    m_pCharRigidBody->setLinearVelocity( output.m_velocity, timestep );

    hkpCharacterStateType state = m_pCharacterContext->getState();

    /*
    switch(state)
    {
    case HK_CHARACTER_ON_GROUND:
        printf("On Ground\n"); break;
    case HK_CHARACTER_JUMPING:
        printf("Jumping\n"); break;
    case HK_CHARACTER_IN_AIR:
        printf("In Air\n"); break;
    case HK_CHARACTER_CLIMBING:
        printf("Climbing\n"); break;
    default:
        printf("Other\n"); break;
    }
    */
}

Ogre::SceneNode* PhysicsRBCharacter::getNode()
{
    return m_pNode;
}

unsigned int PhysicsRBCharacter::GetState()
{
    hkpCharacterStateType state = m_pCharacterContext->getState();

    return state;
}

}// namespace Xk