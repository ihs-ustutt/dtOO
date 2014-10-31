CHECK_DTOO_LIBRARIES(dtOOGeoGen)

SET(HEADERS
  geoGen.h
)
SET(SOURCES
  geoGen.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel 
                    ../libraryKernel ../meshKernel ../optimizationKernel ../dtOOAbstractModule)
ADD_COVISE_MODULE(designTool dtOOGeoGen ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOGeoGen coApi coAppl coCore ${EXTRA_LIBS} 
                      dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOGeoGen)
