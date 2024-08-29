option(BUILD_DOC "Build documents with Doxygen." OFF)
if(NOT BUILD_DOC)
    return()
endif()

find_package(Doxygen REQUIRED)
add_custom_target(doc ALL
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../
    COMMENT "Generating documentation with Doxygen" VERBATIM
)
install(DIRECTORY html DESTINATION doc)
install(FILES
    Hierarchy_And_Modules-librf21x.png Introduction_For_Building.pdf
    DESTINATION doc)
