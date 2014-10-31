CHECK_DTOO_LIBRARIES(dtOOCVGen)

SET(HEADERS
  constValueGenerator.h
)
SET(SOURCES
  constValueGenerator.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../optimizationKernel  ../dtOOAbstractModule)
ADD_COVISE_MODULE(designTool dtOOCVGen ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOCVGen coApi coAppl coCore ${EXTRA_LIBS}
	dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOCVGen)