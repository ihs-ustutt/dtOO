CHECK_DTOO_LIBRARIES(dtOOAGeoToXYZ_qnd)

SET(HEADERS
  aGeoToXYZ.h
)
SET(SOURCES
  aGeoToXYZ.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../optimizationKernel ../dtOOAbstractModule)
ADD_COVISE_MODULE(designTool dtOOAGeoToXYZ_qnd ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOAGeoToXYZ_qnd coApi coAppl coCore ${EXTRA_LIBS}
	                    dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOAGeoToXYZ_qnd)
