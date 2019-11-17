#ifndef USB_HPP
#define USB_HPP

#include <QtCore/QObject>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>

class usb : public QObject
{
    Q_OBJECT
public:
    explicit usb(QObject *parent = nullptr);

signals:
    void deviceAttached(int vid, int pid, const QString &device_name);

public slots:
    void write_command(const QString &device_name);

private:
    QAndroidJniObject *JniTestClass;
    jclass testClass;
    jmethodID _begin, _download;
    QAndroidJniObject m_wakeLock;

};

#endif // USB_HPP
