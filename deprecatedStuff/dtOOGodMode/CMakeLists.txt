CHECK_DTOO_LIBRARIES(dtOOGodMode)

SET(HEADERS
  godMode.h
)
SET(SOURCES
  godMode.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel 
                    ../libraryKernel ../meshKernel ../optimizationKernel ../dtOOAbstractModule)
ADD_COVISE_MODULE(designTool dtOOGodMode ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOGodMode coApi coAppl coCore ${EXTRA_LIBS} 
                      dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOGodMode)
