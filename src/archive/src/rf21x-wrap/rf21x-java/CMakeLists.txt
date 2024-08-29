option(BUILD_WRAP_JAVA "Build API wrap for Java." OFF)
if(NOT BUILD_WRAP_JAVA)
    return()
endif()

find_package(SWIG)
find_package(Java)
find_package(JNI)

include(${SWIG_USE_FILE})

include_directories(../)
include_directories(../../rf21x-c)
include_directories(${JNI_INCLUDE_DIRS})

add_definitions(-DSWIG)

set_source_files_properties(../rf21x.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(../rf21x.i PROPERTIES SWIG_FLAGS "-package;com.enjoyars")
swig_add_module(rf21x-java java ../rf21x.i ../rf21x-wrap.cpp)
swig_link_libraries(rf21x-java ${JNI_LIBRARIES} rf21x-api)

#set_target_properties(rf21x-java PROPERTIES OUTPUT_NAME "rf21x")
install(TARGETS rf21x-java DESTINATION rf21x-api-wrap-java)
install(FILES
    rf21x_test.java
    DESTINATION rf21x-api-wrap-java)
install(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/
    DESTINATION rf21x-api-wrap-java/com/enjoyars
    FILES_MATCHING PATTERN "*.java"
    PATTERN CMakeFiles EXCLUDE)
