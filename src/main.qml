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
        }
    }

    Button {
        id: button
        width: 300
        height: 100
        text: "aaaaa"
        onClicked: u.getVersion()
    }

    Connections {
        target: Qt.application
        onStateChanged: {
            switch (Qt.application.state) {
            case Qt.ApplicationSuspended:
                console.debug("application suspended")
                break
            case Qt.ApplicationHidden:
                console.debug("Application hidden")
                break
            case Qt.ApplicationActive:
                console.debug("Application active")
                break
            case Qt.ApplicationInactive:
                console.debug("Application inactive")
                break
            }
        }
    }
}
