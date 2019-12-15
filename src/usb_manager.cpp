#include <chrono>
#include <iterator>
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtWidgets/QApplication>
#include "android_device.hpp"
#include "callbacks.hpp"
#include "command.hpp"
#include "usb_manager.hpp"
#include "utility.hpp"

#pragma GCC diagnostic ignored "-Wundefined-func-template"

usb_manager::usb_manager(QObject *parent) : QObject(parent), activity_(QtAndroid::androidActivity()), application_state_(Qt::ApplicationActive) {
    connect(callback_manager::instance(), &callback_manager::device_attached, this, [](auto elm) { qDebug() << elm; }, Qt::DirectConnection);
    connect(callback_manager::instance(), &callback_manager::device_attached, this, &usb_manager::write_status_command, Qt::DirectConnection);
    connect(callback_manager::instance(), &callback_manager::device_detached, this, &usb_manager::device_detached, Qt::DirectConnection);
    connect(this, &usb_manager::status_command_witten, this, &usb_manager::read_status, Qt::QueuedConnection);
    connect(this, &usb_manager::status_command_witten_direct, this, &usb_manager::read_status, Qt::DirectConnection);
    connect(this, &usb_manager::callback_assigned, this, &usb_manager::search_device, Qt::DirectConnection);

    emit callback_assigned();
}

void usb_manager::reset_pairing_info(const QString &device_name) {
    android_device::write_command(QAndroidJniObject::fromString(device_name), command::handshake());
    android_device::write_command(QAndroidJniObject::fromString(device_name), command::reset_pairing());
    wait(std::chrono::milliseconds(500));
    android_device::write_command(QAndroidJniObject::fromString(device_name), command::reboot());
}

void usb_manager::write_status_command(const QString &device_name, int vid, int pid) {
    android_device::write_command(QAndroidJniObject::fromString(device_name), command::status());

    if(application_state() != Qt::ApplicationSuspended) emit status_command_witten(device_name, vid, pid);
    else emit status_command_witten_direct(device_name, vid, pid);
}

void usb_manager::read_status(const QString &device_name, int vid, int pid) {
    auto data = android_device::read_data(QAndroidJniObject::fromString(device_name));

    if(application_state() != Qt::ApplicationSuspended) emit device_initialize(device_name, to_hex(data.crend() - 10, data.crend() - 4, 2), vid, pid);
    else emit device_initialize_direct(device_name, to_hex(data.crend() - 10, data.crend() - 4, 2), vid, pid);
}

void usb_manager::search_device() {
    activity_.callMethod<void>("search_device", "()V");
}

void usb_manager::set_application_state(const Qt::ApplicationState state) noexcept {
    application_state_ = state;
}

void usb_manager::vibrate()
{
    android_device::vibrate();
}

const Qt::ApplicationState &usb_manager::application_state() const noexcept {
    return application_state_;
}

#pragma GCC diagnostic warning "-Wundefined-func-template"
