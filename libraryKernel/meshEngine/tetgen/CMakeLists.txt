add_definitions(TETLIBRARY)

SET(HEADERS
  tetgen.h
)
SET(SOURCES
  tetgen.cxx
  predicates.cxx
)

APPEND_DTOO_SOURCE(libraryKernel/meshEngine/tetgen "${SOURCES}")
APPEND_DTOO_HEADER(libraryKernel/meshEngine/tetgen "${HEADERS}")