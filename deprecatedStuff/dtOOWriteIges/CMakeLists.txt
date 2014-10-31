CHECK_DTOO_LIBRARIES(dtOOWriteIges)

SET(HEADERS
  writeIges.h
)
SET(SOURCES
  writeIges.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../meshKernel ../optimizationKernel ../dtOOAbstractModule)
ADD_COVISE_MODULE(designTool dtOOWriteIges ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOWriteIges coApi coAppl coCore ${EXTRA_LIBS}
	                    dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOWriteIges)
