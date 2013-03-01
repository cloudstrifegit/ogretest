AUX_SOURCE_DIRECTORY(. SRC_LIST)
AUX_SOURCE_DIRECTORY(.. SRC_LIST_EXT)
SET(SRC_LIST ${SRC_LIST} ${SRC_LIST_EXT})

SET(havoklib $ENV{HAVOK_SDK_HOME}/Lib/win32_vs2010/$(ConfigurationName)_multithreaded)
SET(havokinc $ENV{HAVOK_SDK_HOME}/Source)

LINK_DIRECTORIES(
	${havoklib}
)

INCLUDE_DIRECTORIES(
	${havokinc}
)

ADD_EXECUTABLE(testcharctl ${SRC_LIST})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

TARGET_LINK_LIBRARIES(testcharctl
		      hkBase.lib
		      hkCompat.lib
		      hkSceneData.lib
		      hkSerialize.lib
		      hkInternal.lib
		      hkGeometryUtilities.lib
		      hkVisualize.lib
		      hkcdInternal.lib
		      hkcdCollide.lib
		      hkpCollide.lib
		      hkpConstraintSolver.lib
		      hkpDynamics.lib
		      hkpInternal.lib
		      hkpUtilities.lib
		      hkpVehicle.lib
)

TARGET_LINK_LIBRARIES(testcharctl debug OgreMain_d.lib)
TARGET_LINK_LIBRARIES(testcharctl debug OIS_d.lib)
TARGET_LINK_LIBRARIES(testcharctl debug OgreOverlay_d.lib)

TARGET_LINK_LIBRARIES(testcharctl optimized OgreMain.lib)
TARGET_LINK_LIBRARIES(testcharctl optimized OIS.lib)
TARGET_LINK_LIBRARIES(testcharctl optimized OgreOverlay.lib)
