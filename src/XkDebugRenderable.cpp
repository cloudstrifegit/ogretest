#include "XkDebugRenderable.h"
#include <vector>

namespace Xk
{

namespace debug
{

Line::Line( Ogre::Vector3& beginPos, Ogre::Vector3& endPos, std::string strName, Ogre::ColourValue color )
{
    m_pManualObj = new Ogre::ManualObject( strName );
    m_pManualObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    m_beginPos = beginPos;
    m_endPos = endPos;
    m_color = color;

    m_pManualObj->position( m_beginPos );
    m_pManualObj->colour( m_color );
    m_pManualObj->position( m_endPos );

    m_pManualObj->end();
}

void Line::update( Ogre::Vector3& beginPos, Ogre::Vector3& endPos, Ogre::ColourValue color )
{
    if( m_beginPos == beginPos && m_endPos == endPos ) return;

    m_beginPos = beginPos;
    m_endPos = endPos;
    m_color = color;

    m_pManualObj->clear();
    m_pManualObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );
    m_pManualObj->position( m_beginPos );
    m_pManualObj->colour( m_color );
    m_pManualObj->position( m_endPos );

    m_pManualObj->end();
}

RagdollConstraintViewer::RagdollConstraintViewer( std::string strName,
                float fPlaneMin,
                float fPlaneMax,
                float fConeMin,
                Ogre::Vector3 vecPivot,
                Ogre::Vector3 twistAxis,
                Ogre::Vector3 planeAxis,
                float fLen)
{
    m_pManualObj = new Ogre::ManualObject( strName );

    m_fPlaneMin = fPlaneMin;
    m_fPlaneMax = fPlaneMax;
    m_fConeMin = fConeMin;

    m_vecPivot = vecPivot;
    m_twistAxis = twistAxis;
    m_planeAxis = planeAxis;

    m_fLength = fLen;

    _createTwistCone( m_pManualObj );
    _createPlaneMinCone( m_pManualObj );
    _createPlaneMaxCone( m_pManualObj );
}

void RagdollConstraintViewer::_createTwistCone( Ogre::ManualObject* pObj )
{
    m_pManualObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    float x, y, z;
    float fRandius = m_fLength;
    int nFragment = 36;
    Ogre::ColourValue color = Ogre::ColourValue::White;

    x = fRandius / tan( m_fConeMin );
    y = fRandius;
    z = 0;

    Ogre::Vector3 vecPos( x, y, z );

    std::vector<Ogre::Vector3> posArray;
    posArray.push_back( vecPos );

    Ogre::Vector3 lastPos = vecPos;

    float fFragmentAngle = 360.0 / (float)nFragment;

    for( int i=0; i<nFragment; i++) {
        Ogre::Vector3 pos = vecPos;
        Ogre::Quaternion rotate;
        rotate.FromAngleAxis( Ogre::Degree( i*fFragmentAngle ), Ogre::Vector3( 1, 0, 0 ) );

        pos = rotate * pos;
        
        posArray.push_back( pos );

        pObj->position( lastPos );
        pObj->colour( color );
        pObj->position( pos );

        lastPos = pos;
    }

    pObj->position( lastPos );
    pObj->colour( color );
    pObj->position( vecPos );

    for( std::vector<Ogre::Vector3>::iterator it = posArray.begin();
        it != posArray.end();
        it ++)
    {
        pObj->position( 0, 0, 0 );
        pObj->colour( color );
        pObj->position( (*it) );
    }

    pObj->end();
}

void RagdollConstraintViewer::_createPlaneMinCone( Ogre::ManualObject* pObj )
{
    m_pManualObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    float x, y, z;
    float fRandius = m_fLength;
    int nFragment = 36;
    Ogre::ColourValue color = Ogre::ColourValue::Red;

    x = fRandius;
    y = fRandius * tan( m_fPlaneMin );
    z = 0;

    Ogre::Vector3 vecPos( x, -y, z );

    std::vector<Ogre::Vector3> posArray;
    posArray.push_back( vecPos );

    Ogre::Vector3 lastPos = vecPos;

    float fFragmentAngle = 360.0 / (float)nFragment;

    for( int i=0; i<nFragment; i++) {
        Ogre::Vector3 pos = vecPos;
        Ogre::Quaternion rotate;
        rotate.FromAngleAxis( Ogre::Degree( i*fFragmentAngle ), Ogre::Vector3( 0, -1, 0 ) );

        pos = rotate * pos;
        
        posArray.push_back( pos );

        pObj->position( lastPos );
        pObj->colour( color );
        pObj->position( pos );

        lastPos = pos;
    }

    pObj->position( lastPos );
    pObj->colour( color );
    pObj->position( vecPos );

    for( std::vector<Ogre::Vector3>::iterator it = posArray.begin();
        it != posArray.end();
        it ++)
    {
        pObj->position( 0, 0, 0 );
        pObj->colour( color );
        pObj->position( (*it) );
    }

    pObj->end();
}

void RagdollConstraintViewer::_createPlaneMaxCone( Ogre::ManualObject* pObj )
{
    m_pManualObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    float x, y, z;
    float fRandius = m_fLength;
    int nFragment = 36;
    Ogre::ColourValue color = Ogre::ColourValue::White;

    x = fRandius;
    y = fRandius * tan( m_fPlaneMax );
    z = 0;

    Ogre::Vector3 vecPos( x, -y, z );

    std::vector<Ogre::Vector3> posArray;
    posArray.push_back( vecPos );

    Ogre::Vector3 lastPos = vecPos;

    float fFragmentAngle = 360.0 / (float)nFragment;

    for( int i=0; i<nFragment; i++) {
        Ogre::Vector3 pos = vecPos;
        Ogre::Quaternion rotate;
        rotate.FromAngleAxis( Ogre::Degree( i*fFragmentAngle ), Ogre::Vector3( 0, 1, 0 ) );

        pos = rotate * pos;
        
        posArray.push_back( pos );

        pObj->position( lastPos );
        pObj->colour( color );
        pObj->position( pos );

        lastPos = pos;
    }

    pObj->position( lastPos );
    pObj->colour( color );
    pObj->position( vecPos );

    for( std::vector<Ogre::Vector3>::iterator it = posArray.begin();
        it != posArray.end();
        it ++)
    {
        pObj->position( 0, 0, 0 );
        pObj->colour( color );
        pObj->position( (*it) );
    }

    pObj->end();
}

Ogre::ManualObject* BuildDebugAxis( Ogre::SceneManager* pMgr, float fLineLen, std::string strName, float fArrowLen)
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

Ogre::ManualObject* createPlane( Ogre::SceneManager* pMgr, 
                        float fLong, 
                        float fWidth, 
                        std::string strName,
                        Ogre::ColourValue color )
{
    Ogre::ManualObject* pObj = pMgr->createManualObject( strName );
    pObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    float fPosX = fLong / 2;
    float fPosZ = fWidth / 2;

    pObj->position( fPosX, 0, fPosZ );
    pObj->colour( color );
    pObj->position( fPosX, 0, -fPosZ );

    pObj->position( fPosX, 0, -fPosZ );
    pObj->colour( color );
    pObj->position( -fPosX, 0, -fPosZ );

    pObj->position( -fPosX, 0, -fPosZ );
    pObj->colour( color );
    pObj->position( -fPosX, 0, fPosZ );

    pObj->position( -fPosX, 0, fPosZ );
    pObj->colour( color );
    pObj->position( fPosX, 0, fPosZ );

    pObj->end();

    return pObj;
}

Ogre::ManualObject* createBox( Ogre::SceneManager* pMgr,
                        float fHalfSize,
                        std::string strName,
                        Ogre::ColourValue color )
{
    Ogre::ManualObject* pObj = pMgr->createManualObject( strName );
    pObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    //AB
    pObj->position(-fHalfSize, fHalfSize, -fHalfSize);
    pObj->colour( color );
    pObj->position(fHalfSize, fHalfSize, -fHalfSize);
    //BC
    pObj->position(fHalfSize, fHalfSize, -fHalfSize);
    pObj->colour( color );
    pObj->position(fHalfSize, fHalfSize, fHalfSize);
    //CD
    pObj->position(fHalfSize, fHalfSize, fHalfSize);
    pObj->colour( color );
    pObj->position(-fHalfSize, fHalfSize, fHalfSize);
    //DA
    pObj->position(-fHalfSize, fHalfSize, fHalfSize);
    pObj->colour( color );
    pObj->position(-fHalfSize, fHalfSize, -fHalfSize);

    //EF
    pObj->position(-fHalfSize, -fHalfSize, -fHalfSize);
    pObj->colour( color );
    pObj->position(fHalfSize, -fHalfSize, -fHalfSize);
    //FG
    pObj->position(fHalfSize, -fHalfSize, -fHalfSize);
    pObj->colour( color );
    pObj->position(fHalfSize, -fHalfSize, fHalfSize);
    //GH
    pObj->position(fHalfSize, -fHalfSize, fHalfSize);
    pObj->colour( color );
    pObj->position(-fHalfSize, -fHalfSize, fHalfSize);
    //HE
    pObj->position(-fHalfSize, -fHalfSize, fHalfSize);
    pObj->position(-fHalfSize, -fHalfSize, -fHalfSize);

    //AE
    pObj->position(-fHalfSize, fHalfSize, -fHalfSize);
    pObj->colour( color );
    pObj->position(-fHalfSize, -fHalfSize, -fHalfSize);
    //BF
    pObj->position(fHalfSize, fHalfSize, -fHalfSize);
    pObj->colour( color );
    pObj->position(fHalfSize, -fHalfSize, -fHalfSize);
    //CG
    pObj->position(fHalfSize, fHalfSize, fHalfSize);
    pObj->colour( color );
    pObj->position(fHalfSize, -fHalfSize, fHalfSize);
    //DH
    pObj->position(-fHalfSize, fHalfSize, fHalfSize);
    pObj->colour( color );
    pObj->position(-fHalfSize, -fHalfSize, fHalfSize);

    pObj->end();

    return pObj;
}

Ogre::ManualObject* createCone( Ogre::SceneManager* pMgr,
                        float fAngle,
                        float fRandius,
                        std::string strName,
                        Ogre::ColourValue color,
                        int nFragment )
{
    Ogre::ManualObject* pObj = pMgr->createManualObject( strName );
    pObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    //m_pManualObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    float x, y, z;

    x = fRandius;
    y = fRandius / tan( 0.3 );
    z = 0;

    Ogre::Vector3 vecPos( x, y, z );

    std::vector<Ogre::Vector3> posArray;
    posArray.push_back( vecPos );

    Ogre::Vector3 lastPos = vecPos;

    float fFragmentAngle = 360.0 / (float)nFragment;

    for( int i=0; i<nFragment; i++) {
        Ogre::Vector3 pos = vecPos;
        Ogre::Quaternion rotate;
        rotate.FromAngleAxis( Ogre::Degree( i*fFragmentAngle ), Ogre::Vector3( 1, 0, 0 ) );

        pos = rotate * pos;
        
        posArray.push_back( pos );

        pObj->position( lastPos );
        pObj->colour( color );
        pObj->position( pos );

        lastPos = pos;
    }

    pObj->position( lastPos );
    pObj->colour( color );
    pObj->position( vecPos );

    for( std::vector<Ogre::Vector3>::iterator it = posArray.begin();
        it != posArray.end();
        it ++)
    {
        pObj->position( 0, 0, 0 );
        pObj->colour( color );
        pObj->position( (*it) );
    }

    pObj->end();

    return pObj;
}

Ogre::ManualObject* createRagdollConstraintViewer( Ogre::SceneManager* pMgr,
                        float fPlaneMin,
                        float fPlaneMax,
                        float coneMin,
                        Ogre::Vector3 vecPivot,
                        Ogre::Vector3 twistAxis,
                        Ogre::Vector3 planeAxis )
{
    Ogre::ManualObject* pObj = pMgr->createManualObject( "ragdolltest" );
    pObj->begin( "BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST );

    float x, y, z;
    float fRandius = 10;
    int nFragment = 36;
    Ogre::ColourValue color = Ogre::ColourValue::White;

    x = fRandius;
    y = fRandius / tan( coneMin );
    z = 0;

    Ogre::Vector3 vecPos( x, y, z );

    std::vector<Ogre::Vector3> posArray;
    posArray.push_back( vecPos );

    Ogre::Vector3 lastPos = vecPos;

    float fFragmentAngle = 360.0 / (float)nFragment;

    for( int i=0; i<nFragment; i++) {
        Ogre::Vector3 pos = vecPos;
        Ogre::Quaternion rotate;
        rotate.FromAngleAxis( Ogre::Degree( i*fFragmentAngle ), Ogre::Vector3( 0, 1, 0 ) );

        pos = rotate * pos;
        
        posArray.push_back( pos );

        pObj->position( lastPos );
        pObj->colour( color );
        pObj->position( pos );

        lastPos = pos;
    }

    pObj->position( lastPos );
    pObj->colour( color );
    pObj->position( vecPos );

    for( std::vector<Ogre::Vector3>::iterator it = posArray.begin();
        it != posArray.end();
        it ++)
    {
        pObj->position( 0, 0, 0 );
        pObj->colour( color );
        pObj->position( (*it) );
    }

    pObj->end();

    return pObj;
}

}// namespace debug

}// namespace Xk