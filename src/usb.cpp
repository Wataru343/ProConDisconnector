#include <iterator>
#include <initializer_list>
#include <vector>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>
#include <QtAndroidExtras/QtAndroid>
#include "callbacks.hpp"
#include "command.hpp"
#include "usb.hpp"
#include <iostream>
#pragma GCC diagnostic ignored "-Wundefined-func-template"

usb::usb(QObject *parent) : QObject(parent), activity_(QtAndroid::androidActivity()) {
    connect(callback_manager::instance(), &callback_manager::device_attached, this, [&](int vid, int pid, const QString &device_name) {
        qDebug() << device_name;
        emit deviceAttached(vid, pid, device_name);
        write_command(device_name);
    });

    connect(callback_manager::instance(), &callback_manager::device_detached, this, &usb::deviceDetached);
    connect(this, &usb::callback_assigned, this, &usb::search_device, Qt::QueuedConnection);

    emit callback_assigned();
}


void usb::write_command(const QString &device_name) {
    QApplication::processEvents();
    QAndroidJniEnvironment env;

    QAndroidJniObject jstr = QAndroidJniObject::fromString(device_name);

    auto write = [this](auto &&jstr, auto &&generator) {
        QAndroidJniEnvironment env;
        auto command = generator();
        jbyteArray arr = env->NewByteArray(std::size(command));
        jboolean is_copy = true;
        jbyte *buf = env->GetByteArrayElements(arr, &is_copy);

        std::copy(std::begin(command), std::end(command), buf);
        env->SetByteArrayRegion(arr, 0, std::size(command), buf);

        activity_.callMethod<void>("write", "(Ljava/lang/String;[B)V", jstr.template object<jstring>(), arr);
        env->ReleaseByteArrayElements(arr, buf, 0);
    };

    write(jstr, make_handshake_command);
    write(jstr, make_rest_command);
}

void usb::search_device() {
    activity_.callMethod<void>("search_device", "()V");
}

#pragma GCC diagnostic warning "-Wundefined-func-template"
