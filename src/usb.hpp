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
    void callback_assigned();

public slots:
    void write_command(const QString &device_name);
    void search_device();

private:
    QAndroidJniObject activity_;

};

#endif // USB_HPP
