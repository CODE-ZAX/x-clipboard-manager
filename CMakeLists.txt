cmake_minimum_required(VERSION 3.16)

# Set application version - this is the main version that will be used for packaging
set(XCLIPY_VERSION "1.1.0")
set(XCLIPY_VERSION_MAJOR "1")
set(XCLIPY_VERSION_MINOR "1")
set(XCLIPY_VERSION_PATCH "0")

project(Xclipy VERSION ${XCLIPY_VERSION} LANGUAGES CXX)

# Enable Qt meta object compiler and friends
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Universal build (unless overridden)
if(APPLE)
    if(NOT CMAKE_OSX_ARCHITECTURES)
        set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Set source files
set(SOURCES
    src/main.cpp
    src/ClipboardManager.cpp
    src/HistoryWindow.cpp
    src/PreferencesWindow.cpp
    src/GlobalHotkey.cpp
)

# Set header files
set(HEADERS
    include/ClipboardManager.h
    include/HistoryWindow.h
    include/PreferencesWindow.h
    include/GlobalHotkey.h
)

# Set resource files
set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(Xclipy ${SOURCES} ${HEADERS} ${RESOURCES})

# Link Qt6 libraries
target_link_libraries(Xclipy PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Platform-specific settings
if(APPLE)
    target_link_libraries(Xclipy PRIVATE "-framework Carbon")
    set_target_properties(Xclipy PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
        MACOSX_BUNDLE_VERSION "${XCLIPY_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${XCLIPY_VERSION_MAJOR}.${XCLIPY_VERSION_MINOR}"
    )
elseif(WIN32)
    # Windows-specific settings if needed
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(X11 REQUIRED x11)
    target_link_libraries(Xclipy PRIVATE ${X11_LIBRARIES})
    target_include_directories(Xclipy PRIVATE ${X11_INCLUDE_DIRS})
endif()

# Set output directory
set_target_properties(Xclipy PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Install rules
install(TARGETS Xclipy
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# Print version information for debugging
message(STATUS "Building Xclipy version: ${XCLIPY_VERSION}")
message(STATUS "CMake minimum required: ${CMAKE_MINIMUM_REQUIRED_VERSION}")
