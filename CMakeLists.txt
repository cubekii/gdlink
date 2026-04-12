cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(Template VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        master  # or pin to a release tag like v0.32.0
)
FetchContent_MakeAvailable(httplib)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h src/*.hpp)
file(GLOB_RECURSE HTTPSERV CONFIGURE_DEPENDS httpserv/*.cpp httpserv/*.h httpserv/*.hpp)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Add platform-specific code as a static library
add_subdirectory(platform)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HTTPSERV})

# Prevent Windows from including old winsock.h (cpp-httplib needs winsock2.h)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)

# Link platform static library and httplib to the main mod (plain signature for Geode compatibility)
target_link_libraries(${PROJECT_NAME} platform_lib httplib::httplib)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

# Set up dependencies, resources, and link Geode.
setup_geode_mod(${PROJECT_NAME})
