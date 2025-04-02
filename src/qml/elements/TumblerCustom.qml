import QtQuick
import QtQuick.Controls


Tumbler {
    id: control

    font.pointSize: 34
    font.family: montserratBold.name

    background: Item {
        Rectangle {
            opacity: control.enabled ? 0.2 : 0.1
            border.color: "#000000"
            width: parent.width
            height: 1
            anchors.top: parent.top
        }

        Rectangle {
            opacity: control.enabled ? 0.2 : 0.1
            border.color: "#000000"
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }

    delegate: Text {
        text: modelData + 1
        font: control.font
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        opacity: 1.0 - Math.abs(Tumbler.displacement) / (control.visibleItemCount / 2)

        required property var modelData
        required property int index
    }

    Rectangle {
        anchors.horizontalCenter: control.horizontalCenter
        y: control.height * 0.4
        width: 40
        height: 1
        color: "white"
    }

    Rectangle {
        anchors.horizontalCenter: control.horizontalCenter
        y: control.height * 0.6
        width: 40
        height: 1
        color: "white"
    }
}
