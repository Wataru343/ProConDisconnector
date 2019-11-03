#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <libusb-1.0/libusb.h>

int main(int argc, char *argv[]) {
    auto version = libusb_get_version();
    std::cout << version->major << '.' << version->minor << '.' << version->micro << std::endl;
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
