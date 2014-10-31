CHECK_DTOO_LIBRARIES(dtOOSFGen)

SET(HEADERS
  sFGen.h
)
SET(SOURCES
  sFGen.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../optimizationKernel  ../dtOOAbstractModule)
ADD_COVISE_MODULE(designTool dtOOSFGen ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOSFGen coApi coAppl coCore ${EXTRA_LIBS}
	dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOSFGen)
