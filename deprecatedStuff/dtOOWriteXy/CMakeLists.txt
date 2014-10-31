CHECK_DTOO_LIBRARIES(dtOOWriteXy_qnd)

SET(HEADERS
  writeXy.h
)
SET(SOURCES
  writeXy.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel 
                    ../optimizationKernel  ../dtOOAbstractModule)
ADD_COVISE_MODULE(designTool dtOOWriteXy_qnd ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOOWriteXy_qnd coApi coAppl coCore ${EXTRA_LIBS}
	                    dtOO dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOOWriteXy_qnd)
