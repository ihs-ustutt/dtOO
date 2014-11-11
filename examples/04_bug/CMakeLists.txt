cmake_minimum_required(VERSION 2.8)

project (geoGen)

find_path(
  DTOO_ROOTDIR cmake/dtOOHelperMacros.cmake
  PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(
  CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DTOO_ROOTDIR}/cmake/"
)

FIND_PACKAGE(EO COMPONENTS eoutils eo es ga cma)
FIND_PACKAGE(CGAL REQUIRED)
find_package(DTOO REQUIRED)
find_package(Boost COMPONENTS thread date_time program_options filesystem system REQUIRED)
find_package(Qt4 REQUIRED QtCore QtGui QtXml QtNetwork )
find_package(Qt4 COMPONENTS Qt3Support QtWebkit QtScript QtScriptTools QtUiTools)

include(${QT_USE_FILE})

include_directories(
  ${DTOO_INCLUDE_DIR} 
  ${CGAL_INCLUDE_DIR} 
  ${EO_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR} 
  ${ROOT_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/header
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
  main 
  ffMeanline.cpp
  )

target_link_libraries(main  ${DTOO_LIBRARIES} ${EO_LIBRARIES} ${QT_LIBRARIES})