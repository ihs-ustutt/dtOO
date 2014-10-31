CHECK_DTOO_LIBRARIES(dtOOMesherOpt)

SET(HEADERS
  mesherOpt.h
  ../dtOOMesher/mesher.h
)
SET(SOURCES
  mesherOpt.cpp
  ../dtOOMesher/mesher.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../optimizationKernel ../meshKernel 
                    ../dtOOAbstractModule ../dtOOMesher)
ADD_COVISE_MODULE(designTool dtOOMesherOpt ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOMesherOpt coApi coAppl coCore ${EXTRA_LIBS}
	                    dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOMesherOpt)

ADD_DEFINITIONS(-DDTOO_NOMAIN)
