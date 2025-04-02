import QtQuick
import QtQuick.Controls

import byhat.logic

import "../elements"


Item {

    anchors.fill: parent

    property double scaleWidthCoef:  0.8
    property double scaleHeightCoef: 0.7

    InputNumberDialog {
        id: numpad

        anchors.centerIn: parent

        onValueUpdate: {
            clckText.value = numpad.value
        }
    }

    Grid {
        id: mainGrid

        property string txtColor: "white"

        width:  parent.width  * scaleWidthCoef
        height: parent.height * scaleHeightCoef

        columns: 2
        spacing: 60
        anchors.centerIn: parent

        horizontalItemAlignment: Grid.AlignHCenter
        verticalItemAlignment:   Grid.AlignVCenter


        Text {
            text: "Текст"

            font.pointSize: 34
            font.family: montserratBold.name

            color: mainGrid.txtColor

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ClickableText {
            id: clckText
            value: 100
            onClicked: numpad.open();
        }

        Text {
            text: "Настройки"

            font.pointSize: 34
            font.family: montserratBold.name

            color: mainGrid.txtColor

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ActionButton {
            text: "Сохранить"
            onClicked: app.saveSettings()
        } 
    }
}
