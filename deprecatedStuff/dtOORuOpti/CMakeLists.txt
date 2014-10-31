CHECK_DTOO_LIBRARIES(dtOORuOpti)

SET(HEADERS
  ruOpti.h
)
SET(SOURCES
  ruOpti.cpp
)

INCLUDE_DIRECTORIES(../generalKernel ../geometryKernel ../libraryKernel ../meshKernel 
                    ../optimizationKernel  ../dtOOAbstractModule)
ADD_COVISE_MODULE(designTool dtOORuOpti ${SOURES} ${HEADERS})
TARGET_LINK_LIBRARIES(dtOORuOpti coApi coAppl coCore ${EXTRA_LIBS}
	logMe functionHeaven dtXmlParser dtXmlParserDecorator dtOOAbstractModule)
COVISE_INSTALL_TARGET(dtOORuOpti)
