find_package(Doxygen)

if (DOXYGEN_FOUND AND DTOO_BUILD_DOC)
  #
  #
  # set doxygen input file
  #
  #
  set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

  #
  #
  # add documentation target
  #
  #
  add_custom_target( 
    doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM 
  )

  #
  #
  # add createInclude target
  #
  #
  add_custom_target(
    createInclude ALL
      COMMAND 
        python createInclude.py
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  #
  #
  # convert each classdtoo_1_1*.xml file with doxy2swig.py
  #
  #
  file(GLOB files "xml/classdtOO_1_1*.xml")
  foreach(file ${files})
    get_filename_component(_fName ${file} NAME)
    add_custom_target(
      ${_fName} ALL
      COMMAND 
        python doxy2swig.py ${file} "${file}.i"
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    #
    #
    # set dependencies for each doxy2swig conversion and createInclude target
    #
    #
    add_dependencies(${_fName} doc_doxygen)
    add_dependencies(createInclude ${_fName})
  endforeach()
    
else (DOXYGEN_FOUND AND DTOO_BUILD_DOC)
  #
  #
  # add createInclude target 
  # also if doxygen not found or documentation not built
  #
  #
  add_custom_target(
    createInclude ALL
      COMMAND rm -f swig_includeAll.i
      COMMAND touch swig_includeAll.i
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif (DOXYGEN_FOUND AND DTOO_BUILD_DOC)
