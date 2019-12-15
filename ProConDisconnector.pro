QT += androidextras quick

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/android_device.hpp \
    src/callbacks.hpp \
    src/command.hpp \
    src/switch_controller.hpp \
    src/switch_controller_model.hpp \
    src/usb_manager.hpp \
    src/utility.hpp

SOURCES += \
    src/android_device.cpp \
    src/main.cpp \
    src/switch_controller_model.cpp \
    src/usb_manager.cpp

RESOURCES += \
    src/qml.qrc

DISTFILES += \
    .gitignore \
    LICENSE \
    README.md \
    src/main.qml

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

android: {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/src/android

    DISTFILES += \
        src/android/.gradle/4.6/fileChanges/last-build.bin \
        src/android/.gradle/4.6/fileHashes/fileHashes.bin \
        src/android/.gradle/4.6/fileHashes/fileHashes.lock \
        src/android/.gradle/vcsWorkingDirs/gc.properties \
        src/android/.project \
        src/android/.settings/org.eclipse.buildship.core.prefs \
        src/android/AndroidManifest.xml \
        src/android/build.gradle \
        src/android/gradle/wrapper/gradle-wrapper.jar \
        src/android/gradle/wrapper/gradle-wrapper.properties \
        src/android/gradlew \
        src/android/gradlew.bat \
        src/android/res/values/libs.xml \
        src/android/res/xml/device_filter.xml \
        src/android/src/com/usb/ProConDisconnectActivity.java \
        src/android/src/com/usb/SwitchController.java
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

