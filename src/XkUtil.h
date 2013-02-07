#ifndef XK_UTIL_H
#define XK_UTIL_H

#include <Ogre.h>
#include <Common/Base/hkBase.h>

namespace Xk
{

static Ogre::Vector3 toVector3(const hkVector4& vec)
{
    return Ogre::Vector3(vec(0), vec(1), vec(2));
}

static hkVector4 tohkVector4(const Ogre::Vector3& vec)
{
    return hkVector4(vec.x, vec.y, vec.z);
}

static hkQuaternion tohkQuaternion(const Ogre::Quaternion& q)
{
	hkQuaternion quat;
	quat.set(q.x, q.y, q.z, q.w);

	return quat;
}

static Ogre::Quaternion toQuaternion(const hkQuaternion& q)
{
    return Ogre::Quaternion(q(3), q(0), q(1), q(2));
}

}//namespace Xk

#endif