cmake_minimum_required(VERSION 3.16)

project(QTemplateApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Qml Gui SerialPort Concurrent REQUIRED)


# Специфичная для Orange Pi библиотека
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
    find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
    find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)
endif()

add_subdirectory(src)
