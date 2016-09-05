DTOO_FIND_LIBRARY_EXT(
  "IGL_FOUND" "igl/igl_inline.h" "libigl.a" 
  "IGL_INCLUDE_DIR" "IGL_LINK_DIRECTORY" "IGL_LIBRARIES" 
  "IGL_FIND_COMPONENTS"
  "/mnt/opt.net/src/libigl/include" "/mnt/opt.net/src/libigl/lib"
)

#
# cork header
#
DTOO_FIND_LIBRARY_HEADERONLY_EXT(
  "CORK_FOUND" "cork.h"
  "CORK_INCLUDE_DIR"
  "/mnt/opt.net/src/libigl/external/cork/src"
)

#
# eigen library
#
find_package(EIGEN REQUIRED)

set(
  IGL_INCLUDE_DIR 
  "${IGL_INCLUDE_DIR};${CORK_INCLUDE_DIR};${EIGEN_INCLUDE_DIR}"
)

#
# static build
#
add_definitions(-DIGL_STATIC_LIBRARY)