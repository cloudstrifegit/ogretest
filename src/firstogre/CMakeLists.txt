AUX_SOURCE_DIRECTORY(. SRC_LIST)
SET(SRC_LIST ${SRC_LIST} ../XkOgreApplication.cpp)

ADD_EXECUTABLE(firstogre ${SRC_LIST})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

TARGET_LINK_LIBRARIES(firstogre debug OgreMain_d.lib)
TARGET_LINK_LIBRARIES(firstogre debug OIS_d.lib)
TARGET_LINK_LIBRARIES(firstogre debug OgreOverlay_d.lib)

TARGET_LINK_LIBRARIES(firstogre optimized OgreMain.lib)
TARGET_LINK_LIBRARIES(firstogre optimized OIS.lib)
TARGET_LINK_LIBRARIES(firstogre optimized OgreOverlay.lib)
