CHECK_DTOO_LIBRARIES(blockMesher)

SET(HEADERS
  blockMesher.h
  dtBmDevelopStuff.h
  dtBmGrid.h
)
SET(SOURCES
  blockMesher.cpp
  dtBmDevelopStuff.cpp
  dtBmGrid.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../optimizationKernel ../dtOOAbstractModule ../meshKernel)
ADD_COVISE_MODULE(designTool blockMesher ${EXTRASOURCES})
TARGET_LINK_LIBRARIES(blockMesher coApi coAppl coCore ${EXTRA_LIBS}
	                      dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(blockMesher)
#prevents error assertion violation
#ADD_DEFINITIONS(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)
