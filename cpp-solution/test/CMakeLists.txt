CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(NAME airport-simulator-test)
project(${NAME})


###############################################################################
#  external library - GTEST
###############################################################################

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# gtest issue on cygwin - https://github.com/google/googletest/issues/2227
set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()

###############################################################################
#  current project settings
###############################################################################

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 14)

#  TODO: it is failing when these options are set on cygwin
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra")

include_directories(
    "${gtest_SOURCE_DIR}/include"
    "${gmock_SOURCE_DIR}/include"
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

ADD_EXECUTABLE ( ${NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Airplane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Airport.cpp
)

target_link_libraries( ${NAME}
    ${CMAKE_BINARY_DIR}/googletest-build/googlemock/gtest/libgtest.a
    ${CMAKE_BINARY_DIR}/googletest-build/googlemock/gtest/libgtest_main.a
    ${CMAKE_BINARY_DIR}/googletest-build/googlemock/libgmock.a
    ${CMAKE_BINARY_DIR}/googletest-build/googlemock/libgmock_main.a
)
