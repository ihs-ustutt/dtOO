CHECK_DTOO_LIBRARIES(dtOOMesher)

SET(HEADERS
  mesher.h
)
SET(SOURCES
  mesher.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../meshKernel ../dtOOAbstractModule
                    ../optimizationKernel)
ADD_COVISE_MODULE(designTool dtOOMesher ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOMesher coApi coAppl coCore ${EXTRA_LIBS}
	                    dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOMesher)
