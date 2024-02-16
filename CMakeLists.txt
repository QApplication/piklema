cmake_minimum_required(VERSION 3.16)

project(Piklema VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Mqtt Qml Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appPiklema
    main.cpp
    # custommodel.h custommodel.cpp
)

target_link_libraries(appPiklema PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Mqtt
    Qt::Qml
    Qt::Quick
)

qt_add_qml_module(appPiklema
    URI Piklema
    VERSION 1.0
    QML_FILES
        Main.qml BasicItem.qml Element.qml FooterItem.qml HeaderItem.qml LogItem.qml
    SOURCES
        custommodel.h custommodel.cpp
    RESOURCES
        "Screenshot 2024-02-13 155904.jpg"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPiklema PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPiklema
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS appPiklema
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
