import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    id: btn

    property string text: text
    property int textSize: 28
    property int border_width: 4
    property color buttonTextColor: "#778cdd"
    property color buttonColorPressed: "#aae1fd"
    property color buttonColorHover: "#c9edfd"
    property color buttonColor: "#f1fbfe"
    property url imageUrl: imageUrl
    property bool btnActive: true
    readonly property alias pressed: btnMouse.pressed

    height: 100
    width: 270

    signal clicked()

    Rectangle {
        id: btnRectangle

        anchors.fill: parent

        color: {
            "white"

            if (btnMouse.containsMouse) {
                if(btnMouse.pressed) {
                    return buttonColorPressed
                }
                else {
                    colorAnim.start()
                    return  btnRectangle.color
                }
            }

            else {
                unhoverAnim.start()
                return btnRectangle.color

            }
        }

        border.color: btnMouse.containsMouse ? (btnMouse.pressed ? buttonColor : buttonColorPressed) : buttonColorHover
        border.width: border_width
        radius: 16

        PropertyAnimation {
            id: colorAnim;

            target: btnRectangle;
            property: "color";
            from: btn.buttonColor;
            to: btn.buttonColorHover;
            duration: 300
        }

        PropertyAnimation {
             id: unhoverAnim;

             target: btnRectangle;
             property: "color";
             from: btn.color;
             to: btn.buttonColor;
             duration: 300
        }

        Text {
            id: btnText

            anchors.fill: parent

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: btn.text

            font.pointSize: btn.textSize
            font.family: montserratBold.name

            color: buttonTextColor
        }

        MouseArea {
            id: btnMouse

            anchors.fill: parent
            enabled: btn.btnActive
            hoverEnabled: true
            onClicked: btn.clicked()
            cursorShape: Qt.PointingHandCursor
        }
    }
}
