cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Qml Gui SerialPort Concurrent LabsSettings REQUIRED)

# Специфичная для Orange Pi библиотека
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
    find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
    find_path(WIRINGPI_INCLUDE_DIRS NAMES wiringPi.h)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)
endif()

qt_add_executable(${PROJECT_NAME}
    main.cpp
    AppEngine.hpp
    AppEngine.cpp
    common/AsyncLogger.hpp
    common/ConfigReader.hpp
    common/MessagesHandler.hpp
    common/structures.hpp
    common/FileHelper.hpp
    common/QMsgHandler.hpp
)

add_definitions(-lwiringPi -lpthread)

qt_add_qml_module(
    ${PROJECT_NAME}
    URI AppQml
    VERSION 1.0

    QML_FILES
        qml/Main.qml

        # gui working pages
        qml/pages/ConfigPage.qml
        qml/pages/InfoPage.qml
        qml/pages/FirstPage.qml

        # gui elements
        qml/elements/ChoosePageBtn.qml
        qml/elements/ActionButton.qml
        qml/elements/RadioBtn.qml
        qml/elements/CmbBox.qml
        qml/elements/TumblerCustom.qml
        qml/elements/SliderCustom.qml
        qml/elements/MessageDialog.qml

        # numpad
        qml/elements/ClickableText.qml
        qml/elements/InputNumberDialog.qml

    RESOURCES
        # qml images
        qml/images/home.svg
        qml/images/setup.svg
        qml/images/info.svg
        qml/images/chooseBtnBlur.svg

        # qml fonts
        qml/fonts/Montserrat/Montserrat-Italic-VariableFont_wght.ttf
        qml/fonts/Montserrat/Montserrat-VariableFont_wght.ttf
        qml/fonts/Montserrat/static/Montserrat-SemiBold.ttf
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Qml
        Qt6::Gui
        Qt6::SerialPort
        Qt6::Concurrent
)

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            ${WIRINGPI_LIBRARIES}
        )
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
