import QtQuick
import QtQuick.Controls

import byhat.logic

import "../elements"


Item {
    anchors.fill: parent

    property double scaleCoef: 0.7

    Grid {
        id: mainGrid

        property string txtColor: "white"

        width:  parent.width * scaleCoef
        height: parent.height * 0.8
        columns: 2

        columnSpacing: 250
        rowSpacing:  60

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

        Column {
            RadioBtn {
                text: "Один"
            }

            RadioBtn {
                text: "Два"
            }
        }

        Text {
            text: "Текст"

            font.pointSize: 34
            font.family: montserratBold.name

            color: mainGrid.txtColor

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        CmbBox {
            width: 300

            model: [10, 200, 3000]
        }

        Text {
            text: "Текст"

            font.pointSize: 34
            font.family: montserratBold.name

            color: mainGrid.txtColor

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        TumblerCustom {
            model: 24
        }

        ActionButton {
            text: "Ок"
        }

        ActionButton {
            text: "Отмена"
        }
    }
}
