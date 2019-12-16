#include <algorithm>
#include "switch_controller_model.hpp"

switch_controller_model::switch_controller_model(QObject *parent): QAbstractListModel(parent), list_(), application_state_(Qt::ApplicationActive) {
}

switch_controller *switch_controller_model::data()
{
    return list_.data();
}

void switch_controller_model::push_back(const switch_controller &elm)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    list_ << elm;
    endInsertRows();
}

void switch_controller_model::refresh() noexcept {
    beginResetModel();
    endResetModel();
}

//MACアドレスが重複していないなら追加する
void switch_controller_model::add_if_not_exist(const QString &device_name, const QString &mac_address, int vid, int pid) {
    for(int i = 0; i < rowCount(); i++) {
        if((data() + i)->mac_address == mac_address.toStdString()) {
            //同じデバイスが5秒以上経過したあと接続された場合再接続されたものとみなす
            if((std::chrono::steady_clock::now() - (data() + i)->atached_time) >= std::chrono::seconds(10)) {
                removeRow(i);
                break;
            }

            (data() + i)->device_name = device_name.toStdString();
            (data() + i)->is_rebooted = true;

            if(application_state() != Qt::ApplicationSuspended) emit device_added_twice(device_name);
            else emit device_added_twice_direct(device_name);

            return;
        }
    }

    switch_controller controller {};
    controller.device_name = device_name.toStdString();
    controller.mac_address = mac_address.toStdString();
    controller.pid = pid;
    controller.vid = vid;
    controller.atached_time = std::chrono::steady_clock::now();
    controller.is_rebooted = false;

    push_back(controller);

    if(application_state() != Qt::ApplicationSuspended) emit device_added(device_name);
    else emit device_added_direct(device_name);
}

//既に再起動済みなら削除する
void switch_controller_model::remove_if_already_rebooted(const QString &device_name) {
    for(int i = 0; i < rowCount(); i++) {
        if((data() + i)->is_rebooted) {
            if((data() + i)->device_name == device_name.toStdString()) {
                removeRow(i);
                return;
            }
        }
    }
}

void switch_controller_model::set_application_state(const Qt::ApplicationState state) noexcept {
    application_state_ = state;
}

const Qt::ApplicationState &switch_controller_model::application_state() const noexcept {
    return application_state_;
}

//渡したリストに無い要素を削除
void switch_controller_model::remove_if_not(const QVector<QString> &device_list) {
    auto result = std::remove_if(std::begin(list_), std::end(list_), [&](const decltype(list_)::value_type &elm){
        return std::none_of(std::begin(device_list), std::end(device_list), [&](const QString &name) {
            return name.toStdString() == elm.device_name;
        });
    });

    if(result != std::end(list_)) {
        list_.erase(result, std::end(list_));
        refresh();
    }
}

int switch_controller_model::rowCount(const QModelIndex &parent) const {
    (void)parent;
    return list_.size();
}

QVariant switch_controller_model::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) return QVariant();

    if(role == TYPEROLE) return QString::fromStdString(list_[index.row()].device_name);
    else if(role == MACADDRESSROLE) return QString::fromStdString(list_[index.row()].mac_address);

    return QVariant();
}

QHash<int, QByteArray> switch_controller_model::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TYPEROLE] = "type";
    roles[MACADDRESSROLE] = "macAddress";

    return roles;
}


bool switch_controller_model::removeRows(int row, int count, const QModelIndex &parent)
{
    (void)parent;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    list_.remove(row, count);
    endRemoveRows();

    return true;
}
