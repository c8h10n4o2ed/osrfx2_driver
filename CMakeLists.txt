cmake_minimum_required (VERSION 2.8)
project (osrfx_usbdriver)

FILE(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

###############################################################################
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
#SET(BUILD_SHARED_LIBRARIES OFF)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")
###############################################################################

add_executable (
    osrfx2
    src/osrfx2.c
)

target_include_directories (
    osrfx2 PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries (
    osrfx2
    usb-1.0
)

###############################################################################
# Set CFLAGS:
# -Wformat=0        Disable string format type checking on printf, etc..
###############################################################################
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Wformat=0")

#set(CMAKE_CXX_FLAGS_DEBUG "-Wformat=0")
#set(CMAKE_CXX_FLAGS_MINSIZEREL "put your flags")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "put your flags")
#set(CMAKE_CXX_FLAGS_RELEASE "-Wformat=0")
###############################################################################
