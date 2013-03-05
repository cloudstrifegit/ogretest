#ifndef XK_DEBUG_RENDERABLE_H
#define XK_DEBUG_RENDERABLE_H

#include <Ogre.h>
#include <Common/Base/hkBase.h>

namespace Xk
{
    namespace debug
    {
        class Line
        {
        public:
            Line( Ogre::Vector3& beginPos, Ogre::Vector3& endPos, std::string strName = "", Ogre::ColourValue color = Ogre::ColourValue::White );

            void update( Ogre::Vector3& beginPos, Ogre::Vector3& endPos, Ogre::ColourValue color = Ogre::ColourValue::White );

            Ogre::ManualObject* getManualObject() { return m_pManualObj; }

        private:
            Ogre::Vector3 m_beginPos;
            Ogre::Vector3 m_endPos;
            Ogre::ColourValue m_color;
            Ogre::ManualObject* m_pManualObj;
        };

        class RagdollConstraintViewer
        {
        public:
            RagdollConstraintViewer( std::string strName, 
                float fPlaneMin,
                float fPlaneMax,
                float fConeMin,
                Ogre::Vector3 vecPivot ,
                Ogre::Vector3 twistAxis = Ogre::Vector3::UNIT_X,
                Ogre::Vector3 planeAxis = Ogre::Vector3::UNIT_Z,
                float fLen = 1.0 );

            void update( float fPlaneMin,
                float fPlaneMax,
                float fConeMin,
                Ogre::Vector3 vecPivot,
                Ogre::Vector3 twistAxis = Ogre::Vector3::UNIT_X,
                Ogre::Vector3 planeAxis = Ogre::Vector3::UNIT_Z,
                float fLen = 1.0 );
            
            Ogre::ManualObject* getManualObject() { return m_pManualObj; }

        private:
            void _createTwistCone( Ogre::ManualObject* pObj );
            void _createPlaneMinCone( Ogre::ManualObject* pObj );
            void _createPlaneMaxCone( Ogre::ManualObject* pObj );

        private:
            float m_fPlaneMin;
            float m_fPlaneMax;
            float m_fConeMin;
            Ogre::Vector3 m_vecPivot;
            Ogre::Vector3 m_twistAxis;
            Ogre::Vector3 m_planeAxis;

            Ogre::ManualObject* m_pManualObj;

            float m_fLength;
        };

        Ogre::ManualObject* createAxis( Ogre::SceneManager* pMgr, 
                        float fLineLen, 
                        std::string strName, 
                        float fArrowLen = 0.2 );

        Ogre::ManualObject* createPlane( Ogre::SceneManager* pMgr, 
                        float fLong, 
                        float fWidth, 
                        std::string strName,
                        Ogre::ColourValue color = Ogre::ColourValue( 1.0f, 1.0f, 1.0f ) );

        Ogre::ManualObject* createBox( Ogre::SceneManager* pMgr,
                        float fHalfSize,
                        std::string strName,
                        Ogre::ColourValue color = Ogre::ColourValue( 0.0f, 0.0f, 0.0f ) );

        Ogre::ManualObject* createCone( Ogre::SceneManager* pMgr,
                        float fAngle,
                        float fRandius,
                        std::string strName,
                        Ogre::ColourValue color = Ogre::ColourValue::White,
                        int nFragment = 36);

        Ogre::ManualObject* createRagdollConstraintViewer( Ogre::SceneManager* pMgr,
                        float fPlaneMin,
                        float fPlaneMax,
                        float coneMin,
                        Ogre::Vector3 vecPivot,
                        Ogre::Vector3 twistAxis,
                        Ogre::Vector3 planeAxis );

    }//namespace debug

}//namespace Xk

#endif