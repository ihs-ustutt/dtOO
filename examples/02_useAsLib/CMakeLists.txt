cmake_minimum_required(VERSION 2.8)

project (dtOO)

find_path(
  DTOO_ROOTDIR cmake/dtOOHelperMacros.cmake
  PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}
)


set(
  CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DTOO_ROOTDIR}/cmake/"
)

find_package(CGAL REQUIRED)
find_package(ROOT REQUIRED)
find_package(DTOO REQUIRED)
find_package(EO COMPONENTS eoutils eo es ga cma REQUIRED)
find_package(Boost REQUIRED)

include_directories(
  ${DTOO_INCLUDE_DIR} ${CGAL_INCLUDE_DIR} ${EO_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR} ${ROOT_INCLUDE_DIR}
)

add_executable(main main.cpp)
target_link_libraries(main ${DTOO_LIBRARIES})