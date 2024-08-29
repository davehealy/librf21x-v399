option(BUILD_API_EXAMPLE "Build API example program for testing." ON)
if(NOT BUILD_API_EXAMPLE)
    return()
endif()

include_directories(../src/rf21x-c)
add_executable(rf21x-api-test rf21x-api-test.cpp)
target_link_libraries(rf21x-api-test rf21x-api)
install(TARGETS rf21x-api-test
    RUNTIME DESTINATION rf21x-api)
install(FILES rf21x-api-test.cpp
    DESTINATION rf21x-api)


#add_executable(rf21x-api-test-mutiple rf21x-api-test-mutiple.cpp)
#target_link_libraries(rf21x-api-test-mutiple rf21x-api)
#install(TARGETS rf21x-api-test-mutiple
#    RUNTIME DESTINATION rf21x-api)


add_executable(rf228-auth-test rf228-auth-test.cpp)
target_link_libraries(rf228-auth-test rf21x-api)
install(TARGETS rf228-auth-test
    RUNTIME DESTINATION rf21x-api)
install(FILES rf228-auth-test.cpp
    DESTINATION rf21x-api)




add_executable(rf228-connection-test rf228-connection-test.cpp)
target_link_libraries(rf228-connection-test rf21x-api)
install(TARGETS rf228-connection-test
    RUNTIME DESTINATION rf21x-api)
install(FILES rf228-connection-test.cpp
    DESTINATION rf21x-api)
