option(BUILD_WRAP_CSHARP "Build API wrap for C#." OFF)
if(NOT BUILD_WRAP_CSHARP)
    return()
endif()


find_package(SWIG)

include(${SWIG_USE_FILE})

include_directories(../)
include_directories(../../rf21x-c)

add_definitions(-DSWIG)

set_source_files_properties(../rf21x.i PROPERTIES CPLUSPLUS ON)
swig_add_module(rf21x-csharp csharp ../rf21x.i ../rf21x-wrap.cpp)
swig_link_libraries(rf21x-csharp rf21x-api)

set_target_properties(rf21x-csharp PROPERTIES OUTPUT_NAME "rf21x")
install(TARGETS rf21x-csharp DESTINATION rf21x-api-wrap-csharp)
install(FILES
    rf21x-test.cs
    DESTINATION rf21x-api-wrap-csharp)
install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}
    DESTINATION rf21x-api-wrap-csharp
    FILES_MATCHING REGEX ".+\\.cs$"
    PATTERN CMakeFiles EXCLUDE)
