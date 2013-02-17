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

static Ogre::ManualObject* BuildDebugBox(Ogre::SceneManager* pMgr, float fHalfSize, std::string strName, std::string strMatName)
{
    Ogre::ManualObject* pObj = pMgr->createManualObject(strName);
    pObj->begin(strMatName, Ogre::RenderOperation::OT_LINE_LIST);

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

    pObj->end();

    return pObj;
}

}//namespace Xk

#endif