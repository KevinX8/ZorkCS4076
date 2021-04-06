cmake_minimum_required(VERSION 3.16.0)

project(ZorkUI LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

FILE(GLOB_RECURSE FILEMGMTSOURCES FileManagement/**.cpp FileManagement/**.h)
FILE(GLOB_RECURSE INTERACTIONSOURCES interaction/**.cpp interaction/**.h)
FILE(GLOB_RECURSE GAMEWORLDSOURCES GameWorld/**.cpp GameWorld/**.h)
FILE(GLOB_RECURSE QTGUISOURCES QTGUI/**.cpp QTGUI/**.h)

set(PROJECT_SOURCES
        main.cpp        
        QTGUI/zorkgui.ui
        ${FILEMGMTSOURCES}
        ${INTERACTIONSOURCES}
        ${GAMEWORLDSOURCES}
        ${QTGUISOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ZorkUI
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(ZorkUI SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ZorkUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(ZorkUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
