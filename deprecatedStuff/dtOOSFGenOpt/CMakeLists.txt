CHECK_DTOO_LIBRARIES(dtOOSFGenOpt)

SET(HEADERS
  sFGenOpt.h
  ../dtOOSFGen/sFGen.h
)
SET(SOURCES
  sFGenOpt.cpp
  ../dtOOSFGen/sFGen.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../optimizationKernel ../dtOOAbstractModule ../dtOOSFGen)
ADD_COVISE_MODULE(designTool dtOOSFGenOpt ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOSFGenOpt coApi coAppl coCore ${EXTRA_LIBS}
	                    dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOSFGenOpt)

ADD_DEFINITIONS(-DDTOO_NOMAIN)
