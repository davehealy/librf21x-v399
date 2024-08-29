if(MSVC)
    add_definitions(-DLIBRF21X_EXPORTS)
endif()

add_library(rf21x-api SHARED rf21x-api.cpp)
target_link_libraries(rf21x-api rf21x)
include_directories(../rf21x)

install(TARGETS rf21x-api DESTINATION rf21x-api)
install(FILES rf21x-api.h DESTINATION rf21x-api)
