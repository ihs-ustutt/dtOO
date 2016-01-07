SET(HEADERS
  dtArg.h
)
SET(SOURCES
  dtArg.cpp
)

APPEND_DTOO_SOURCE(applicationKernel "${SOURCES}")
APPEND_DTOO_HEADER(applicationKernel "${HEADERS}")