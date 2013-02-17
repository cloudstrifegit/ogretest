#ifndef XK_STEP_LISTENER_H
#define XK_STEP_LISTENER_H

#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

namespace Xk
{

class StepListener
{
public:
    virtual bool step(float fTime) = 0;
    virtual hkpRigidBody* getRigidBody()
    {
        return m_rigidBody;
    }
    
protected:
    hkpRigidBody* m_rigidBody;
};

}//namespace Xk

#endif