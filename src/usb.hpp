#ifndef USB_HPP
#define USB_HPP

#include <QObject>

class usb : public QObject
{
    Q_OBJECT
public:
    explicit usb(QObject *parent = nullptr);

signals:

public slots:
};

#endif // USB_HPP
