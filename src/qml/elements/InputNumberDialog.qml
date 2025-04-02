import QtQuick
import QtQuick.Controls

import byhat.logic


Dialog {
    id: dlg

    property string value: "0"
    property int maxInputValues: 7
    property int maxValue: 100000
    property int minValue: 0

    signal valueUpdate()

    modal: true

    function updateValue(character) {
        var currentValue = dlg.value

        if (currentValue.length > dlg.maxInputValues) {
            return
        }

        currentValue += character

        if (parseInt(currentValue) > dlg.maxValue) {
            dlg.value = dlg.maxValue
            return
        }

        dlg.value = currentValue
    }

    contentItem: Rectangle {
        color: "white"
        anchors.fill: parent
        radius: 2

        Column {
            anchors.centerIn: parent
            spacing: 20

            Rectangle {
                color: "#f2f2f2"

                width:  headerTxt.width * 1.1
                height: headerTxt.height * 1.1

                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    id: headerTxt
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Значение ИЗП"
                    font.pointSize: 34
                    font.family: montserratBold.name

                    color: "#778cdd"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle {
                border.color: "#3155c4"
                radius: 20

                width:  headerTxt.width
                height: headerTxt.height

                anchors.horizontalCenter: parent.horizontalCenter

                Text {
                    anchors.fill: parent
                    text: {
                        if (dlg.value === "") {
                            return "0"
                        }

                        return dlg.value
                    }

                    font.pointSize: 34
                    font.family: montserratBold.name

                    color: "#778cdd"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Grid {
                anchors.horizontalCenter: parent.horizontalCenter

                columns: 3
                columnSpacing: 20
                rowSpacing: 20

                Repeater {
                    model: [7, 8, 9, 4, 5, 6, 1, 2, 3]
                    delegate: ActionButton {
                        width: 100
                        text: modelData.toString()
                        onClicked: {
                            dlg.updateValue(text)
                        }
                    }
                }
            }

            Row {
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter

                ActionButton {
                    width: 100

                    text: "0"

                    onClicked: {
                        dlg.updateValue("0")
                    }
                }

                ActionButton {
                    text: "C"
                    width: 100
                    onClicked: dlg.value = 0
                }

                ActionButton {
                    width: 100

                    textSize: 50
                    text: "←"
                    onClicked: {
                        var currentValue = dlg.value

                        if (currentValue.length > 0) {
                            currentValue = currentValue.slice(0, -1)
                        } else {
                            currentValue = 0
                        }

                        dlg.value = currentValue
                    }
                }
            }

            Row {
                spacing: 15
                anchors.horizontalCenter: parent.horizontalCenter

                ActionButton {
                    text: "Отмена"
                    width: 160
                    height: 70
                    textSize: 17
                    onClicked:
                    {
                        var value = parseInt(dlg.value)

                        if (value > dlg.maxValue) {
                            dlg.value = dlg.maxValue
                        }

                        if (value < dlg.minValue) {
                            dlg.value = dlg.minValue
                        }

                        dlg.close();
                    }
                }

                ActionButton {
                    text: "Применить"
                    width: 160
                    height: 70
                    textSize: 17
                    onClicked: {
                        var value = parseInt(dlg.value)

                        if (value > dlg.maxValue) {
                            dlg.value = dlg.maxValue
                        }

                        if (value < dlg.minValue) {
                            dlg.value = dlg.minValue
                        }

                        dlg.valueUpdate();
                        dlg.close();
                    }
                }
            }
        }
    }
}
