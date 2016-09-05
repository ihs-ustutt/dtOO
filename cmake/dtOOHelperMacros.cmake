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
  git_describe(DTOO_VERSION --tags)

  #parse the version information into pieces.
  string(
    REGEX REPLACE 
    "^([a-z/A-Z]+)[0-9]+\\..*" "\\1" DTOO_VERSION_NAME 
    "${DTOO_VERSION}"
  )
  string(
    REGEX REPLACE 
    "^[a-z/A-Z]+([0-9]+)\\..*" "\\1" DTOO_VERSION_MAJOR 
    "${DTOO_VERSION}"
  )
  string(
    REGEX REPLACE 
    "^[a-z/A-Z]+[0-9]+\\.([0-9]+).*" "\\1" DTOO_VERSION_MINOR 
    "${DTOO_VERSION}"
  )
  if("${DTOO_VERSION}" MATCHES "-")
    string(
      REGEX REPLACE 
      "^[a-z/A-Z]+[0-9]+\\.[0-9]+\\-([0-9]+).*" "\\1" DTOO_VERSION_COMMIT 
      "${DTOO_VERSION}"
    )
    string(
      REGEX REPLACE 
      "^[a-z/A-Z]+[0-9]+\\.[0-9]+\\-[0-9]+\\-([0-9]+).*" "\\1" DTOO_VERSION_SHA1 
      "${DTOO_VERSION}"
    )
  else("${DTOO_VERSION}" MATCHES "-")
    set(DTOO_VERSION_COMMIT "0")
    set(DTOO_VERSION_SHA1 "")
  endif("${DTOO_VERSION}" MATCHES "-")
  configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/dtOOVersion.h.in
    ${CMAKE_SOURCE_DIR}/src/generalKernel/dtOOVersion.h
  )

  add_custom_target(
      version ALL
      DEPENDS ${CMAKE_SOURCE_DIR}/src/generalKernel/dtOOVersion.h
  )

  message(STATUS "|----------------------------------------------------")
  message(
    STATUS 
    "| Compiling dtOO ${DTOO_VERSION_NAME}"
    "${DTOO_VERSION_MAJOR}.${DTOO_VERSION_MINOR}.${DTOO_VERSION_COMMIT}"
  )
  message(STATUS "|----------------------------------------------------")
  set(VERSION_MAJOR "${DTOO_VERSION_MAJOR}")
  set(VERSION_MINOR "${DTOO_VERSION_MINOR}")
  set(VERSION_COMMIT "${DTOO_VERSION_COMMIT}")
ENDMACRO(MAKE_DTOO_VERSION)

MACRO(
  DTOO_FIND_LIBRARY_EXT
  LIB_FOUND TESTINCNAME TESTLIBNAME 
  LIB_INCLUDE_DIR LIB_LINK_DIRECTORY LIB_LIBRARIES
  LIB_FIND_COMPONENTS
  LIB_ADD_INCSEARCHPATH LIB_ADD_LIBSEARCHPATH
)
  IF( NOT ${LIB_FOUND} STREQUAL TRUE )
    set( _testincname ${TESTINCNAME} )
    set( _testlibname ${TESTLIBNAME} )
    set( 
      _incsearchpath 
      /usr/ihs/include/
      ${LIB_ADD_INCSEARCHPATH}
    )
    set( 
      _libsearchpath 
      /usr/ihs/lib/
      /usr/ihs/lib64/
      ${LIB_ADD_LIBSEARCHPATH}
    )
    
    #find the include dir by looking for moab/Version.h
    FIND_PATH( 
      ${LIB_INCLUDE_DIR} ${_testincname}
      PATHS 
      ${_incsearchpath}
    )
    IF( ${LIB_INCLUDE_DIR} STREQUAL ${LIB_INCLUDE_DIR}-NOTFOUND )
      SET( ${LIB_FOUND} FALSE CACHE BOOL FORCE )
      MESSAGE( FATAL_ERROR "Cannot find ${_testincname} include dir." )
    ENDIF( ${LIB_INCLUDE_DIR} STREQUAL ${LIB_INCLUDE_DIR}-NOTFOUND )

    # Find one lib and save its directory to LIB_LINK_DIRECTORY.
    FIND_PATH( 
      ${LIB_LINK_DIRECTORY} ${_testlibname} 
      PATH 
      ${_libsearchpath} 
    )

    IF( ${LIB_LINK_DIRECTORY} STREQUAL ${_testlibname}-NOTFOUND )
      SET( ${LIB_FOUND} FALSE CACHE BOOL FORCE )
      MESSAGE( FATAL_ERROR "Cannot find ${_testlibname}." )
    ELSE( ${LIB_LINK_DIRECTORY} STREQUAL ${_testlibname}-NOTFOUND )
      SET( ${LIB_FOUND} TRUE CACHE BOOL "Has been found?" FORCE )
      SET( _firsttime TRUE )
      MESSAGE( 
        STATUS 
        "|\n"
        "| ${LIB_FOUND}=${${LIB_FOUND}}\n"
        "| ${LIB_INCLUDE_DIR}=${${LIB_INCLUDE_DIR}}\n"
        "| ${LIB_LINK_DIRECTORY}=${${LIB_LINK_DIRECTORY}}\n"
        "|" 
        )
    ENDIF( ${LIB_LINK_DIRECTORY} STREQUAL ${_testlibname}-NOTFOUND )
  ELSE( NOT ${LIB_FOUND} STREQUAL TRUE )
    SET( _firsttime FALSE ) #so that messages are only printed once
  ENDIF( NOT ${LIB_FOUND} STREQUAL TRUE )

  IF( ${LIB_FOUND} STREQUAL TRUE )
    IF( DEFINED ${LIB_FIND_COMPONENTS} )
      FOREACH( _libname ${${LIB_FIND_COMPONENTS}} )
        #look for libs in MOAB_LINK_DIRECTORY
        FIND_LIBRARY( 
          ${_libname}_LIB
          ${_libname} 
          ${${LIB_LINK_DIRECTORY}}
          NO_DEFAULT_PATH
        )
        SET( _foundlib ${${_libname}_LIB} )
        IF( _foundlib STREQUAL ${_libname}_LIB-NOTFOUND )
          MESSAGE( 
            FATAL_ERROR 
            "Cannot find ${_libname}. Is it spelled correctly? Correct \
             capitalization? Do you have another package with similarly-named \
             libraries, installed at ${LIB_LINK_DIRECTORY}?" 
          )
        ENDIF( _foundlib STREQUAL ${_libname}_LIB-NOTFOUND )
        SET( ${LIB_LIBRARIES} ${${LIB_LIBRARIES}} ${_foundlib} )
      ENDFOREACH( _libname ${${LIB_FIND_COMPONENTS}} )
    ELSE( DEFINED ${LIB_FIND_COMPONENTS} )
      MESSAGE( 
       AUTHOR_WARNING 
       "Developer must specify required libraries to link against in the cmake \
        file, i.e. find_package( LIB REQUIRED COMPONENTS lib1 lib2) . \
        Otherwise no libs will be added - linking against ALL libraries is \
        slow!"
      )
    ENDIF( DEFINED ${LIB_FIND_COMPONENTS} )
  ENDIF( ${LIB_FOUND} STREQUAL TRUE )
ENDMACRO()

MACRO(
  DTOO_FIND_LIBRARY
  LIB_FOUND TESTINCNAME TESTLIBNAME 
  LIB_INCLUDE_DIR LIB_LINK_DIRECTORY LIB_LIBRARIES
  LIB_FIND_COMPONENTS
)

DTOO_FIND_LIBRARY_EXT(
  ${LIB_FOUND} ${TESTINCNAME} ${TESTLIBNAME}
  ${LIB_INCLUDE_DIR} ${LIB_LINK_DIRECTORY} ${LIB_LIBRARIES}
  ${LIB_FIND_COMPONENTS}
  "" ""
)

ENDMACRO()

MACRO(
  DTOO_FIND_LIBRARY_HEADERONLY_EXT
  LIB_FOUND TESTINCNAME
  LIB_INCLUDE_DIR
  LIB_ADD_INCSEARCHPATH
)
  IF( NOT ${LIB_FOUND} STREQUAL TRUE )
    set( _testincname ${TESTINCNAME} )
    set( 
      _incsearchpath 
      /usr/ihs/include/
      ${LIB_ADD_INCSEARCHPATH}
    )
    
    #find the include dir by looking for moab/Version.h
    FIND_PATH( 
      ${LIB_INCLUDE_DIR} ${_testincname}
      PATHS 
      ${_incsearchpath}
    )
    IF( ${LIB_INCLUDE_DIR} STREQUAL ${LIB_INCLUDE_DIR}-NOTFOUND )
      SET( ${LIB_FOUND} FALSE CACHE BOOL FORCE )
      MESSAGE( FATAL_ERROR "Cannot find ${_testincname} include dir." )
    ENDIF( ${LIB_INCLUDE_DIR} STREQUAL ${LIB_INCLUDE_DIR}-NOTFOUND )


      SET( ${LIB_FOUND} TRUE CACHE BOOL "Has been found?" FORCE )
      SET( _firsttime TRUE )
      MESSAGE( 
        STATUS 
        "|\n"
        "| ${LIB_FOUND}=${${LIB_FOUND}}\n"
        "| ${LIB_INCLUDE_DIR}=${${LIB_INCLUDE_DIR}}\n"
        "|" 
        )
  ELSE( NOT ${LIB_FOUND} STREQUAL TRUE )
    SET( _firsttime FALSE ) #so that messages are only printed once
  ENDIF( NOT ${LIB_FOUND} STREQUAL TRUE )
ENDMACRO()
