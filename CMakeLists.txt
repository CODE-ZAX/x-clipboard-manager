cmake_minimum_required(VERSION 3.16)
project(ClipboardManager VERSION 1.2.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.12 COMPONENTS Core Gui Widgets REQUIRED)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Source files
set(SOURCES
    src/main.cpp
    src/ClipboardManager.cpp
    src/HistoryWindow.cpp
    src/PreferencesWindow.cpp
    src/GlobalHotkey.cpp
)

# Header files
set(HEADERS
    include/ClipboardManager.h
    include/HistoryWindow.h
    include/PreferencesWindow.h
    include/GlobalHotkey.h
)

# Resource files
set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(ClipboardManager ${SOURCES} ${HEADERS} ${RESOURCES})

# Include directories
target_include_directories(ClipboardManager PRIVATE include)

# Link Qt libraries
target_link_libraries(ClipboardManager PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

# Platform-specific libraries for global hotkeys
if(APPLE)
    find_library(CARBON_LIBRARY Carbon)
    target_link_libraries(ClipboardManager PRIVATE ${CARBON_LIBRARY})
elseif(WIN32)
    # Windows doesn't need additional libraries for global hotkeys
elseif(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(ClipboardManager PRIVATE ${X11_LIBRARIES})
    target_include_directories(ClipboardManager PRIVATE ${X11_INCLUDE_DIR})
endif()

# Platform-specific settings
if(APPLE)
    set_target_properties(ClipboardManager PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist"
    )
endif()

# Install rules
install(TARGETS ClipboardManager
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# Optional: Install resources
install(DIRECTORY resources/
    DESTINATION share/ClipboardManager
    FILES_MATCHING PATTERN "*.qrc"
)

# Optional: Install documentation
install(FILES README.md
    DESTINATION share/doc/ClipboardManager
)
