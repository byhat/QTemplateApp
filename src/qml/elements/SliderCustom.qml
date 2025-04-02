import QtQuick
import QtQuick.Controls

Slider
{
    id: control

    from: 100
    to: 1000000
    stepSize:  100

    value: 100

    onMoved: console.log(value)

    background: Rectangle
    {
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2

        implicitWidth: 350
        implicitHeight: 15

        width: control.availableWidth
        height: implicitHeight

        radius: this.height / 2

        color: "#bdbebf"

        Rectangle
        {
            width: control.visualPosition * parent.width
            height: parent.height

            color: "#69e8ff"

            radius: this.height / 2
        }
    }

    handle: Rectangle
    {
        x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2

        implicitWidth:  40
        implicitHeight: 40

        radius: this.width / 2

        color: control.pressed ? "#aae1fd" : "#f1fbfe"

        border.color: "#bdbebf"
    }
}
