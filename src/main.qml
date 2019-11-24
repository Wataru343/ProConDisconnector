import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import Usb 1.0

Window {
    id: root
    title: qsTr("ProCon Disconnect")
    visible: true

    Usb {
        id: usb
        onDeviceAttached: {
            model.append({ device_name: device_name })
        }

        onDeviceDetached: {
            for(var i = 0; i < model.count; i++)
                if(model.get(i).device_name === device_name)
                    model.remove(model.get(i));
        }
    }

    ListModel {
        id: model
    }

    ListView {
        width: root.width
        height: root.height

        model: model
        delegate: Rectangle {
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.1
            anchors.right: parent.right
            anchors.rightMargin: parent.width * 0.1
            height: text.height

            Text {
                id: text
                text: device_name
            }
        }

        ScrollBar.vertical: ScrollBar {
            active: true
        }
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
