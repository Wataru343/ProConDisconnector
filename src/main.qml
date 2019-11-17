import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import Usb 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Usb {
        id: u
        onDeviceAttached: {
            button.text = device_name
            write_command(device_name)
        }
    }

    Button {
        id: button
        width: 300
        height: 100
        text: "aaaaa"
        onClicked: u.getVersion()
    }
}
