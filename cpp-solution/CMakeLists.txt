CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(NAME airport-simulator)
project(${NAME})


###############################################################################
#  current project settings
###############################################################################

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra")


###############################################################################
#  building main executable
###############################################################################

remove_definitions(-D USE_GTEST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_EXECUTABLE ( ${NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
