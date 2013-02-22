AUX_SOURCE_DIRECTORY(. SRC_LIST)
SET(SRC_LIST ${SRC_LIST} ../XkOgreApplication.cpp)

ADD_EXECUTABLE(thirdcamera ${SRC_LIST})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

TARGET_LINK_LIBRARIES(thirdcamera debug OgreMain_d.lib)
TARGET_LINK_LIBRARIES(thirdcamera debug OIS_d.lib)
TARGET_LINK_LIBRARIES(thirdcamera debug OgreOverlay_d.lib)

TARGET_LINK_LIBRARIES(thirdcamera optimized OgreMain.lib)
TARGET_LINK_LIBRARIES(thirdcamera optimized OIS.lib)
TARGET_LINK_LIBRARIES(thirdcamera optimized OgreOverlay.lib)
