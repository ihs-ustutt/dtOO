CHECK_DTOO_LIBRARIES(dtOOOptimizer)

SET(HEADERS
  cvOptimizer.h
)
SET(SOURCES
  cvOptimizer.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../meshKernel ../dtOOAbstractModule
                    ../optimizationKernel)
ADD_COVISE_MODULE(designTool dtOOCVOptimizer ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOCVOptimizer coApi coAppl coCore ${EXTRA_LIBS}
	                    dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOCVOptimizer)
