#include <iterator>
#include <initializer_list>
#include <vector>
#include <QtWidgets/QApplication>
#include <QtAndroidExtras/QtAndroid>
#include <libusb-1.0/libusb.h>
#include "callbacks.hpp"
#include "command.hpp"
#include "usb.hpp"

#pragma GCC diagnostic ignored "-Wundefined-func-template"

usb::usb(QObject *parent) : QObject(parent), activity_(QtAndroid::androidActivity()) {
    connect(callback_manager::instance(), &callback_manager::device_attached, this, [&](int vid, int pid, const QString &device_name) {
        qDebug() << device_name;
        emit deviceAttached(vid, pid, device_name);
        write_command(device_name);
    }, Qt::DirectConnection);

    connect(this, &usb::callback_assigned, this, &usb::search_device, Qt::QueuedConnection);

    emit callback_assigned();
}


void usb::write_command(const QString &device_name) {
    QApplication::processEvents();
    QAndroidJniEnvironment env;
    QAndroidJniObject jstr = QAndroidJniObject::fromString(device_name);
    activity_.callMethod<void>("write", "(Ljava/lang/String;)V", jstr.object<jstring>());
}

void usb::search_device() {
    activity_.callMethod<void>("search_device", "()V");
}

#pragma GCC diagnostic warning "-Wundefined-func-template"
