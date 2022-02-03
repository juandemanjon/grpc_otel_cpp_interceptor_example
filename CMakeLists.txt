cmake_minimum_required(VERSION 3.22)

# vcpkg settings must be set before project()
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

message("CMAKE_TOOLCHAIN_FILE " ${CMAKE_TOOLCHAIN_FILE})

# In Manifest mode CMake invokes vcpkg automatically This makes setup easier, however, in CI or Docker we may want to
# turn this off
option(VCPKG_MANIFEST_MODE "Build vcpkg ports from manifest" ON)

# Ensure we install vcpkg ports in the same place so they can be reused between builds
set(_VCPKG_INSTALLED_DIR
    "${CMAKE_CURRENT_LIST_DIR}/vcpkg_installed"
    CACHE STRING "")

project(grpc-interceptor-example VERSION 0.1.0)

find_package(gRPC CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

add_executable(grpc-interceptor-example main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_reflection)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
