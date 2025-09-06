# CMakeList.txt : CMake project for Divisors, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (CMAKE_BUILD_TYPE MATCHES "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()

project (Divisors CXX)

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.10.1
)
FetchContent_MakeAvailable(pybind11)

find_package(Python COMPONENTS Development REQUIRED)
#find_package(primesieve REQUIRED)

#find_library(LIB_PRIMESIEVE primesieve HINTS include)
add_library(Divisors STATIC "src/Divisors.cpp" "include/Point.h" "src/Point.cpp" "include/ValidInteger.h" "include/Builtins.h")
#add_executable (Divisors "Divisors.cpp" "Divisors.h")


include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    include
)

target_include_directories(Divisors PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/python
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
    include
    include/python
)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    lib
)

#target_link_libraries(SH_module PRIVATE Python::Python)
#target_link_libraries(Divisors primesieve)
#target_link_libraries(Divisors include/primsieve.lib)
target_link_libraries(Divisors primesieve)

install(TARGETS Divisors DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Divisors PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
