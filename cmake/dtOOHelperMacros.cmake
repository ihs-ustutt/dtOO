MACRO(APPEND_DTOO_SOURCE DIRNAME FILES)
  foreach(FILE ${FILES})
    list(APPEND LIST ${DIRNAME}/${FILE})
  endforeach(FILE)
  set(DTOO_SOURCES ${DTOO_SOURCES};${LIST} PARENT_SCOPE)
ENDMACRO(APPEND_DTOO_SOURCE)

MACRO(APPEND_DTOO_HEADER DIRNAME FILES)
  foreach(FILE ${FILES})
    list(APPEND LIST ${DIRNAME}/${FILE})
  endforeach(FILE)
  set(DTOO_HEADERS ${DTOO_HEADERS};${LIST} PARENT_SCOPE)
ENDMACRO(APPEND_DTOO_HEADER)

MACRO(MAKE_DTOO_VERSION VERSION_MAJOR VERSION_MINOR VERSION_COMMIT)
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE DTOO_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  include(GetGitRevisionDescription)
  git_describe(DTOO_VERSION --tags --dirty)

  #parse the version information into pieces.
  string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" DTOO_VERSION_MAJOR "${DTOO_VERSION}")
  string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" DTOO_VERSION_MINOR "${DTOO_VERSION}")
  string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\-([0-9]+).*" "\\1" DTOO_VERSION_COMMIT "${DTOO_VERSION}")
  string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\-[0-9]+\\-([0-9]+).*" "\\1" DTOO_VERSION_SHA1 "${DTOO_VERSION}")

  configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/dtOOVersion.h.in
    ${CMAKE_SOURCE_DIR}/generalKernel/dtOOVersion.h
  )

  add_custom_target(
      version ALL
      DEPENDS ${CMAKE_SOURCE_DIR}/generalKernel/dtOOVersion.h
  )

  message(STATUS "|")
  message(STATUS "| Compiling dtOO v${DTOO_VERSION_MAJOR}.${DTOO_VERSION_MINOR}.${DTOO_VERSION_COMMIT}")
  message(STATUS "|")
  set(VERSION_MAJOR "${DTOO_VERSION_MAJOR}")
  set(VERSION_MINOR "${DTOO_VERSION_MINOR}")
  set(VERSION_COMMIT "${DTOO_VERSION_COMMIT}")
ENDMACRO(MAKE_DTOO_VERSION)

MACRO(CREATE_DTOO_DOC)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    add_custom_target(
      doc ALL
      ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/Doxyfile
      COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
  endif(DOXYGEN_FOUND)
ENDMACRO(CREATE_DTOO_DOC)
