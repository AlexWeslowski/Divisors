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

if (MSVC)
    add_compile_options(/W4)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Werror -Winit-self)
endif()

project (Divisors CXX)

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.10.1
)
FetchContent_MakeAvailable(pybind11)


get_filename_component(PARENT_DIR "${PROJECT_SOURCE_DIR}" DIRECTORY)
get_filename_component(PARENT_DIR "${PARENT_DIR}" DIRECTORY)
set(REPOS
    "cppalliance/int128/include/boost;boost"
    "jbapple/128-bit-literals;128-bit-literals"
    "int128-libraries/abslint128/include;abslint128"
    "int128-libraries/abslint128/src;abslint128"
    "abseil/abseil-cpp/absl/numeric;absl"
)
foreach(REPO ${REPOS}) 
    string(REPLACE ";" " " ATUPLE "${REPO}")
    separate_arguments(ALIST UNIX_COMMAND "${ATUPLE}")
    list(GET ALIST 0 ASTRING)
    list(GET ALIST 1 BSTRING)
    
    string(REPLACE "/" " " BTUPLE "${ASTRING}")
    separate_arguments(BLIST UNIX_COMMAND "${BTUPLE}")
    list(GET BLIST 0 CSTRING)
    list(GET BLIST 1 DSTRING)

    string(REPLACE ";" " " ALIST "${REPO}")
    string(REPLACE "/" " " BLIST "${ALIST_0}")
    
    message(STATUS "ASTRING = ${ASTRING}")
    message(STATUS "BSTRING = ${BSTRING}")
    message(STATUS "CSTRING = ${CSTRING}")
    message(STATUS "DSTRING = ${DSTRING}")
    message(STATUS "ALIST_0 = ${ALIST_0}")
    message(STATUS "ALIST_1 = ${ALIST_1}")
    message(STATUS "BLIST_0 = ${BLIST_0}")
    message(STATUS "BLIST_1 = ${BLIST_1}")

    set(CLONE_DIR "${PARENT_DIR}/${CSTRING}")
    file(MAKE_DIRECTORY "${CLONE_DIR}")
	if(NOT EXISTS "${CLONE_DIR}/${DSTRING}/.git")
		message(STATUS "Cloning repo into ${CLONE_DIR}")
		execute_process(
			COMMAND git clone --depth 1 https://github.com/${CSTRING}/${DSTRING}.git "${CLONE_DIR}"
			RESULT_VARIABLE clone_result
			OUTPUT_VARIABLE clone_output
			ERROR_VARIABLE clone_error
		)
		if(NOT clone_result EQUAL 0)
			message(FATAL_ERROR "Git clone failed:\n${clone_error}")
		endif()
	else()
		message(STATUS "Repo already exists at ${CLONE_DIR}, skipping clone.")
	endif()
	set(CLONE_DIR "${PARENT_DIR}/${ASTRING}/")
    message(STATUS "Clone from directory ${CLONE_DIR}")
    message(STATUS "Clone to directory ${PROJECT_SOURCE_DIR}/include/${BSTRING}")
	file(COPY ${CLONE_DIR} DESTINATION "${PROJECT_SOURCE_DIR}/include/${BSTRING}")
endforeach()


set(BASE_URL "https://github.com/gcc-mirror/gcc/tree/master/gcc")
set(FILES "wide-int.cc" "wide-int.h" "wide-int-bitmask.h" "wide-int-print.cc" "wide-int-print.h")
foreach(FILE ${FILES})
    set(URL "${BASE_URL}/${FILE}")
    set(DEST "${PROJECT_SOURCE_DIR}/include/gcc/${FILE}")
    get_filename_component(DEST_DIR ${DEST} DIRECTORY)
    file(MAKE_DIRECTORY ${DEST_DIR})
    file(DOWNLOAD ${URL} ${DEST}
        STATUS status
        SHOW_PROGRESS
        LOG log)
endforeach()


find_package(Python COMPONENTS Development REQUIRED)
#find_package(primesieve REQUIRED)

set(INCLUDE_FILES "include/Globals.h" "include/Point.h" "include/ValidInteger.h" "include/Builtins.h" "include/ArrayArray.h" "include/Combinations.h" "include/Functions.h" "include/CombinationsIterator.h")
set(SRC_FILES "src/Globals.cpp" "src/Divisors.cpp" "src/Point.cpp" "src/ArrayArray.cpp" "src/Combinations.cpp" "src/CombinationsIterator.cpp")

#find_library(LIB_PRIMESIEVE primesieve HINTS include)
#add_library(Divisors STATIC  "include/Globals.h" "src/Globals.cpp" "src/Divisors.cpp" "include/Point.h" "src/Point.cpp" "include/ValidInteger.h" "include/Builtins.h" "src/ArrayArray.cpp" "include/ArrayArray.h" "src/Combinations.cpp" "include/Combinations.h" "include/Functions.h" "src/CombinationsIterator.cpp" "include/CombinationsIterator.h")
add_library(Divisors STATIC ${SRC_FILES} ${INCLUDE_FILES})
#add_library(ArrayArray STATIC "include/Globals.h" "src/Globals.cpp" "include/ValidInteger.h" "src/ArrayArray.cpp" "include/ArrayArray.h")
#add_library(Globals STATIC "include/Globals.h" "src/Globals.cpp")
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

#pybind11_add_module(ArrayArray "include/ValidInteger.h" "include/ArrayArray.h" "src/ArrayArray.cpp")
#pybind11_add_module(DivisorsModule ${SRC_FILES})

#target_link_libraries(SH_module PRIVATE Python::Python)
#target_link_libraries(Divisors primesieve)
#target_link_libraries(Divisors include/primsieve.lib)
target_link_libraries(Divisors primesieve) 
#target_link_libraries(Globals PRIVATE Divisors)
#target_link_libraries(Globals PRIVATE ArrayArray)
#target_link_libraries(ArrayArray PRIVATE Divisors)



install(TARGETS Divisors DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Divisors PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
