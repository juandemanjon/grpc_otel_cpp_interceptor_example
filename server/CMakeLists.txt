cmake_minimum_required(VERSION 3.22)

find_package(gRPC CONFIG REQUIRED)
find_package(Threads)

include(CTest)
enable_testing()
     
add_executable(${PROJECT_NAME} main.cpp)
            
target_link_libraries(${PROJECT_NAME} PRIVATE myproto gRPC::gpr gRPC::grpc++_reflection)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
