SET(HEADERS
  dtArg.h
)
SET(SOURCES
  dtArg.cpp
)

APPEND_DTOO_SOURCE(src/applicationKernel "${SOURCES}")
APPEND_DTOO_HEADER(src/applicationKernel "${HEADERS}")