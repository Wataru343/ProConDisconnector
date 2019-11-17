#include <iterator>
#include <initializer_list>
#include <vector>
#include <QtWidgets/QApplication>
#include <QtAndroidExtras/QtAndroid>
#include <libusb-1.0/libusb.h>
#include "callbacks.hpp"
#include "command.hpp"
#include "usb.hpp"
#include <QDebug>

#pragma GCC diagnostic ignored "-Wundefined-func-template"

usb::usb(QObject *parent) : QObject(parent) {
    connect(callback_manager::instance(), &callback_manager::device_attached, this, [&](int vid, int pid, const QString &device_name) {
        qDebug() << device_name;
        emit deviceAttached(vid, pid, device_name);
    });
}


void usb::write_command(const QString &device_name) {
    QApplication::processEvents();
    QAndroidJniEnvironment env;
    QAndroidJniObject jstr = QAndroidJniObject::fromString(device_name);
    QAndroidJniObject activity = QtAndroid::androidActivity();
    activity.callMethod<void>("write", "(Ljava/lang/String;)V", jstr.object<jstring>());
}

#pragma GCC diagnostic warning "-Wundefined-func-template"
