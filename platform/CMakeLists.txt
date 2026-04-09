 if(WIN32)
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS windows/*.cpp windows/*.h windows/*.hpp)
elseif(APPLE)
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS macos/*.cpp macos/*.h macos/*.hpp)
elseif(ANDROID)
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS android/*.cpp android/*.h android/*.hpp)
endif()

add_library(platform_lib STATIC ${PLATFORM_SOURCES})

target_include_directories(platform_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})