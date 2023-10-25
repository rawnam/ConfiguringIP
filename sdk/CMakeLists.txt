cmake_minimum_required(VERSION 3.4.1)

project(configuring_network)

set(SOURCE_FILES
    configuring_network.cpp
    sdk.cpp
    pch.cpp
)

add_library(configuring_network_static STATIC ${SOURCE_FILES})
