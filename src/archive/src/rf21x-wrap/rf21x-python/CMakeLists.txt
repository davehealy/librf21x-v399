option(BUILD_WRAP_PYTHON "Build API wrap for Python." OFF)
if(NOT BUILD_WRAP_PYTHON)
    return()
endif()

find_package(SWIG)
find_package(PythonLibs)

include(${SWIG_USE_FILE})

include_directories(../)
include_directories(../../rf21x-c)
include_directories(${PYTHON_INCLUDE_DIRS})

add_definitions(-DSWIG)

set_source_files_properties(../rf21x.i PROPERTIES CPLUSPLUS ON)
swig_add_module(rf21x-python python ../rf21x.i ../rf21x-wrap.cpp)
swig_link_libraries(rf21x-python ${PYTHON_LIBRARIES} rf21x-api)

set_target_properties(_rf21x-python PROPERTIES OUTPUT_NAME "_rf21x")
install(TARGETS _rf21x-python DESTINATION rf21x-api-wrap-python)
install(FILES
    rf21x-test.py
    ${CMAKE_CURRENT_BINARY_DIR}/rf21x.py
    DESTINATION rf21x-api-wrap-python)
