cmake_minimum_required(VERSION 3.16)
project(vscode-launcher VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Widgets)

# Find yaml-cpp
find_package(yaml-cpp REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/ProjectManager.cpp
    src/LauncherService.cpp
)

set(HEADERS
    src/ProjectManager.h
    src/LauncherService.h
)

# Qt resources
qt_add_resources(RESOURCES resources.qrc)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Widgets
    yaml-cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install desktop file
install(FILES vscode-launcher.desktop
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
)

# Install icon
install(FILES icon.png
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps
    RENAME vscode-launcher.png
)

# Install example config to home directory (custom target)
add_custom_target(install-config
    COMMAND ${CMAKE_SOURCE_DIR}/install-config.sh ${CMAKE_SOURCE_DIR}/config.yaml.example
    COMMENT "Installing example configuration"
)
