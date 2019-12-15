#include <iostream>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include "switch_controller_model.hpp"
#include "usb_manager.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    usb_manager usb;
    switch_controller_model controller_list;

    QObject::connect(&app, &QGuiApplication::applicationStateChanged, &usb, &usb_manager::set_application_state, Qt::DirectConnection);
    QObject::connect(&app, &QGuiApplication::applicationStateChanged, &controller_list, &switch_controller_model::set_application_state, Qt::DirectConnection);

    QObject::connect(&usb, &usb_manager::device_initialize, &controller_list, &switch_controller_model::add_if_not_exist, Qt::QueuedConnection);
    QObject::connect(&usb, &usb_manager::device_initialize_direct, &controller_list, &switch_controller_model::add_if_not_exist, Qt::DirectConnection);

    QObject::connect(&controller_list, &switch_controller_model::device_added, &usb, &usb_manager::reset_pairing_info, Qt::QueuedConnection);
    QObject::connect(&controller_list, &switch_controller_model::device_added_direct, &usb, &usb_manager::reset_pairing_info, Qt::DirectConnection);

    QObject::connect(&usb, &usb_manager::device_detached, &controller_list, &switch_controller_model::remove_if_already_rebooted, Qt::QueuedConnection);
    QObject::connect(&usb, &usb_manager::device_detached_direct, &controller_list, &switch_controller_model::remove_if_already_rebooted, Qt::DirectConnection);

    QObject::connect(&controller_list, &switch_controller_model::device_added_twice, &usb, &usb_manager::vibrate, Qt::QueuedConnection);
    QObject::connect(&controller_list, &switch_controller_model::device_added_twice_direct, &usb, &usb_manager::vibrate, Qt::DirectConnection);


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("switch_controller_model", &controller_list);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
