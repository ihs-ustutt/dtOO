ADD_DEFINITIONS(-DINCLUDE_TEMPLATES)

DTOO_FIND_LIBRARY_EXT(
  "OpenVolumeMesh_FOUND" "OpenVolumeMesh/Core/TopologyKernel.hh" "libOpenVolumeMesh.so" 
  "OpenVolumeMesh_INCLUDE_DIR" "OpenVolumeMesh_LINK_DIRECTORY" "OpenVolumeMesh_LIBRARIES" 
  "OpenVolumeMesh_FIND_COMPONENTS"
  "" "/usr/ihs/lib/OpenVolumeMesh"
)