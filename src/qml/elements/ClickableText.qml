import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: bg

    property int value: 200

    signal clicked()
    signal textChanged()

    width: 300
    height: 60

    radius: 5

    MouseArea {
        anchors.fill: parent
        onClicked: bg.clicked()
    }

    Text {
        text: bg.value

        anchors.fill: parent

        font.pointSize: 34
        font.family: montserratBold.name

        color: "#778cdd"

        onTextChanged: bg.textChanged()

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
