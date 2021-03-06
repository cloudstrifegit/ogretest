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

static Ogre::ManualObject* BuildDebugAxis( Ogre::SceneManager* pMgr, float fLineLen, std::string strName, float fArrowLen = 0.2)
{
    Ogre::ManualObject* pObj = pMgr->createManualObject( strName );
    pObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    //X
    pObj->position( 0, 0, 0 );
    pObj->colour( 1.0f, 0, 0 );
    pObj->position( fLineLen, 0, 0 );

    pObj->position( fLineLen, 0, 0 );
    pObj->colour( 1.0f, 0, 0 );
    pObj->position( fLineLen-fArrowLen, fArrowLen, 0);

    pObj->position( fLineLen, 0, 0 );
    pObj->colour( 1.0f, 0, 0 );
    pObj->position( fLineLen-fArrowLen, -fArrowLen, 0 );

    pObj->position( fLineLen, 0, 0 );
    pObj->colour( 1.0f, 0, 0 );
    pObj->position( fLineLen-fArrowLen, 0, fArrowLen );
    
    pObj->position( fLineLen, 0, 0 );
    pObj->colour( 1.0f, 0, 0 );
    pObj->position( fLineLen-fArrowLen, 0, -fArrowLen );

    //Y
    pObj->position( 0, 0, 0 );
    pObj->colour( 0, 1.0f, 0 );
    pObj->position( 0, fLineLen, 0 );

    pObj->position( 0, fLineLen, 0 );
    pObj->colour( 0, 1.0f, 0 );
    pObj->position( fArrowLen, fLineLen-fArrowLen, 0 );

    pObj->position( 0, fLineLen, 0 );
    pObj->colour( 0, 1.0f, 0 );
    pObj->position( -fArrowLen, fLineLen-fArrowLen, 0);

    pObj->position( 0, fLineLen, 0 );
    pObj->colour( 0, 1.0f, 0 );
    pObj->position( 0, fLineLen-fArrowLen, fArrowLen );

    pObj->position( 0, fLineLen, 0 );
    pObj->colour( 0, 1.0f, 0 );
    pObj->position( 0, fLineLen-fArrowLen, -fArrowLen );

    //Z
    pObj->position( 0, 0, 0 );
    pObj->colour( 0, 0, 1.0f );
    pObj->position( 0, 0, fLineLen );

    pObj->position( 0, 0, fLineLen );
    pObj->colour( 0, 0, 1.0f );
    pObj->position( 0, fArrowLen, fLineLen-fArrowLen );

    pObj->position( 0, 0, fLineLen );
    pObj->colour( 0, 0, 1.0f );
    pObj->position( 0, -fArrowLen, fLineLen-fArrowLen );

    pObj->position( 0, 0, fLineLen );
    pObj->colour( 0, 0, 1.0f );
    pObj->position( fArrowLen, 0, fLineLen-fArrowLen );

    pObj->position( 0, 0, fLineLen );
    pObj->colour( 0, 0, 1.0f );
    pObj->position( -fArrowLen, 0, fLineLen-fArrowLen );

    pObj->end();

    return pObj;
}

static Ogre::ManualObject* BuildDebugPlane( Ogre::SceneManager* pMgr, float fLong, float fWidth, std::string strName )
{
    Ogre::ManualObject* pObj = pMgr->createManualObject( strName );
    pObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    float fPosX = fLong / 2;
    float fPosZ = fWidth / 2;

    pObj->position( fPosX, 0, fPosZ );
    pObj->colour( 1.0f, 1.0f, 1.0f );
    pObj->position( fPosX, 0, -fPosZ );

    pObj->position( fPosX, 0, -fPosZ );
    pObj->colour( 1.0f, 1.0f, 1.0f );
    pObj->position( -fPosX, 0, -fPosZ );

    pObj->position( -fPosX, 0, -fPosZ );
    pObj->colour( 1.0f, 1.0f, 1.0f );
    pObj->position( -fPosX, 0, fPosZ );

    pObj->position( -fPosX, 0, fPosZ );
    pObj->colour( 1.0f, 1.0f, 1.0f );
    pObj->position( fPosX, 0, fPosZ );

    pObj->end();

    return pObj;
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