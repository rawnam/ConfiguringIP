cmake_minimum_required(VERSION 3.10)

project(configuring_network_gui)

# Specify the required Qt modules
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Source files
set(SOURCE_FILES
    configuring_network_gui.cpp
    main.cpp
    QProgressIndicator.cpp
    # ../configuring_ip/configuring_ip.cpp
    # ../sdk/configuring_network.cpp
)

# Header files
set(HEADER_FILES
    configuring_network_gui.h
    QProgressIndicator.h
    resource.h
    # ../sdk/configuring_network.h
)

# UI files
set(UI_FILES
    configuring_network_gui.ui
)

# Resource files
set(RESOURCES
    configuring_network_gui.qrc
)

add_executable(configuring_network_gui ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES} ${RESOURCES})

target_link_libraries(configuring_network_gui Qt5::Core Qt5::Gui Qt5::Widgets)
