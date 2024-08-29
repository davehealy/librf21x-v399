option(BUILD_WRAP_CPP "Build API wrap for C++." ON)
if(BUILD_WRAP_CPP)
    include_directories(../rf21x-c)
    add_executable(rf21x-wrap-test rf21x-wrap.cpp rf21x-wrap-test.cpp)
    target_link_libraries(rf21x-wrap-test rf21x-api)
    install(TARGETS rf21x-wrap-test DESTINATION rf21x-api-wrap-cpp)
    install(FILES
        rf21x-wrap.h rf21x-wrap.cpp rf21x-wrap-test.cpp
        DESTINATION rf21x-api-wrap-cpp)
endif()

add_subdirectory(rf21x-java)
add_subdirectory(rf21x-python)
add_subdirectory(rf21x-csharp)
