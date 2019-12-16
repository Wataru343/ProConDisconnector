#ifndef USB_HPP
#define USB_HPP

#include <QtCore/QObject>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>

class usb_manager : public QObject
{
    Q_OBJECT
public:
    explicit usb_manager(QObject *parent = nullptr);
    const Qt::ApplicationState& application_state() const noexcept;
    QString all_device();

signals:
    void callback_assigned();
    void device_detached(const QString &device_name, int vid, int pid);
    void device_detached_direct(const QString &device_name, int vid, int pid);
    void status_command_witten(const QString &device_name, int vid, int pid);
    void status_command_witten_direct(const QString &device_name, int vid, int pid);
    void device_initialize(const QString &device_name, const QString &mac_address, int vid, int pid);
    void device_initialize_direct(const QString &device_name, const QString &mac_address, int vid, int pid);
    void access_failed(usb_manager *sender);

public slots:
    void write_status_command(const QString &device_name, int vid, int pid);
    void read_status(const QString &device_name, int vid, int pid);
    void search_device();
    void reset_pairing_info(const QString &device_name);
    void set_application_state(const Qt::ApplicationState state) noexcept;
    void vibrate();

private:
    QAndroidJniObject activity_;
    Qt::ApplicationState application_state_;
};

#endif // USB_HPP
