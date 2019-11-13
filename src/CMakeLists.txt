cmake_minimum_required(VERSION 3.5)

project(ProConDisconnector LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")


if(ANDROID)
    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android" CACHE STRING "description")
    add_library(${PROJECT_NAME} SHARED main.cpp usb.cpp qml.qrc)    
else()
    add_executable(${PROJECT_NAME} main.cpp usb.cpp qml.qrc)
endif()

find_package(Qt5 COMPONENTS Core Quick AndroidExtras REQUIRED)
list(GET Qt5Core_INCLUDE_DIRS 0 Qt5_INCLUDE_DIR)
message(STATUS "Found Qt5: ${Qt5_INCLUDE_DIR} (found version \"${Qt5_VERSION}\")")

target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::AndroidExtras)


find_package(LIBUSB-1.0 REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${LIBUSB-1.0_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIBUSB-1.0_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBUSB-1.0_LIBRARIES})


# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# These variables must use CACHE, otherwise QtCreator won't see them.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android" CACHE INTERNAL "")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so CACHE INTERNAL "")
#    endif()
#endif()
