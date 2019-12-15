#ifndef SWITCH_CONTROLLER_MODEL_HPP
#define SWITCH_CONTROLLER_MODEL_HPP

#include <QtCore/QAbstractListModel>
#include <QtCore/QObject>
#include <QtCore/QVector>
#include "switch_controller.hpp"

class switch_controller_model : public QAbstractListModel {
    Q_OBJECT

public:
    enum roles {
        TYPEROLE = Qt::UserRole + 1,
        MACADDRESSROLE,
    };

    explicit switch_controller_model(QObject *parent = nullptr);
    switch_controller* data();
    void push_back(const switch_controller &elm);
    const Qt::ApplicationState& application_state() const noexcept;

signals:
    void device_attached(const QString &device_name, int vid, int pid);
    void device_added(const QString &device_name);
    void device_added_direct(const QString &device_name);
    void device_added_twice(const QString &device_name);
    void device_added_twice_direct(const QString &device_name);

public slots:
    void add_if_not_exist(const QString &device_name, const QString &mac_address, int vid, int pid);
    void remove_if_already_rebooted(const QString &device_name);
    void set_application_state(const Qt::ApplicationState state) noexcept;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index = QModelIndex(), int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

private:
    QVector<switch_controller> list_;
    Qt::ApplicationState application_state_;
};

#endif // SWITCH_CONTROLLER_MODEL_HPP
