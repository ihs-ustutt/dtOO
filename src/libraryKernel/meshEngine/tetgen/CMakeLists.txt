add_definitions(TETLIBRARY)

SET(HEADERS
  tetgen.h
)
SET(SOURCES
  tetgen.cxx
  predicates.cxx
)

APPEND_DTOO_SOURCE(src/libraryKernel/meshEngine/tetgen "${SOURCES}")
APPEND_DTOO_HEADER(src/libraryKernel/meshEngine/tetgen "${HEADERS}")