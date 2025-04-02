import QtQuick
import QtQuick.Controls


RadioButton
{
    id: control

    width: 300
    height: 100

    font.pointSize: 40
    font.family: montserratBold.name

    indicator: Rectangle
    {
        implicitWidth: 26
        implicitHeight: 26
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 13

        Rectangle
        {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 7
            color: control.down ? "#778cdd" : "#5a75d5"
            visible: control.checked
        }
    }

    contentItem: Text
    {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: control.down ? "#ebebebff" : "white"

        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }
}
