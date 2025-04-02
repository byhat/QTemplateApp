import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    id: chooseBtn

    property string imagePath: "qrc:/AppQml/qml/images/info.svg"
    property string btnText:   "Диагностика"

    property string blurPath: "qrc:/AppQml/qml/images/chooseBtnBlur.svg"
    property bool   blurVisible: false

    property int    imageWidth: 200
    property int    textWidth:  200
    property int    textHeight:  60

    signal btnClicked()

    MouseArea {
        anchors.fill: parent
        onClicked: chooseBtn.btnClicked()
    }

    Column {
        anchors.fill: parent
        spacing: 10

        anchors.centerIn: parent

        Image {
            id: bgBlur

            width:  parent.width * 0.8
            height: parent.width * 0.8

            anchors.horizontalCenter: parent.horizontalCenter

            source: (blurVisible)? blurPath : ""

            Image {
                id: btnImage

                anchors.fill: parent
                source: imagePath
            }
        }

        Text {
            width: parent.width * 0.8

            anchors.horizontalCenter: parent.horizontalCenter

            text:  btnText

            color: "white"

            font.pointSize: 14
            font.family: montserratBold.name

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
