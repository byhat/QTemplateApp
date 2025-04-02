import QtQuick
import QtQuick.Controls

import "../elements"


Dialog {
    id: errorDialog

    property string message: "Непредвиденная ошибка"

    width:  msgTxt.width + 20
    height: dlgBtn.height + msgTxt.height + 20

    // height: 200

    modal: true

    contentItem: Rectangle {
        color: "white"
        anchors.fill: parent
        radius: 2
    }

        Column {
            spacing: 15
            anchors.centerIn: parent

            Text {
                id: msgTxt
                text: errorDialog.message

                font.pointSize: 34
                font.family: montserratBold.name

                color: "#778cdd"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            ActionButton {
                id: dlgBtn
                text: "ОК"
                width: 160
                height: 70
                textSize: 17
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: errorDialog.close();
            }
        }
}
