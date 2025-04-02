import QtQuick
import QtQuick.Controls

import "../elements"


Item {
    anchors.fill: parent

    property double scaleCoef: 0.8

    Text {
        anchors.centerIn: parent
        text: "Шаблон программы на Qt/QML/C++"

        font.pointSize: 34
        font.family: montserratBold.name

        color: "white"

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

}
