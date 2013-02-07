#ifndef XK_STEP_LISTENER_H
#define XK_STEP_LISTENER_H

namespace Xk
{

class StepListener
{
public:
    virtual bool step(float fTime) = 0;
};

}//namespace Xk

#endif