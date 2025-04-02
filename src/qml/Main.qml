import QtQml
import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import "./elements"


ApplicationWindow
{
    id: main

    x: 10
    y: 10

    width: 1280
    height: 850

    visible: true
    visibility: (app.m_Fullscreen)? Window.FullScreen : Window.Windowed

    title: qsTr("Template app")

    color: "#304993"

    Settings {
            property alias x: main.x
            property alias y: main.y
            property alias width: main.width
            property alias height: main.height
        }

    FontLoader {
        id: montserratBold;
        source: "qrc:/AppQml/qml/fonts/Montserrat/static/Montserrat-SemiBold.ttf"
    }

    MessageDialog {
        id: msgDialog
        anchors.centerIn: parent
    }

    Rectangle {
        id: choosePageBg

        width: 180
        height: parent.height

        color: "#03276b"

        ListView {
            id: choosePageView

            width: parent.width
            height: parent.height - 50

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter:   parent.verticalCenter

            spacing:  10
            model: choosePageLst
            delegate:

                ChoosePageBtn {
                    width: 180
                    height: 200

                    anchors.horizontalCenter: parent.horizontalCenter

                    imagePath: model.imagePath
                    btnText:   model.btnText

                    onBtnClicked: {
                        loader.loadFragment(model.index);
                        choosePageView.currentIndex = model.index
                    }

                    blurVisible: choosePageView.currentIndex === model.index
                }
        }
    }

    ListModel {
        id: choosePageLst

        ListElement {
            imagePath: "qrc:/AppQml/qml/images/home.svg"
            btnText:   "Главная\nстраница"
        }

        ListElement {
            imagePath: "qrc:/AppQml/qml/images/setup.svg"
            btnText:   "Настройки"
        }

        ListElement {
            imagePath: "qrc:/AppQml/qml/images/info.svg"
            btnText:   "О программе"
        }
    }

    Loader {
        id: loader

        width: parent.width - choosePageView.width
        height: parent.height

        anchors.left: choosePageBg.right

        source: "./pages/FirstPage.qml"

        function loadFragment(index) {
            switch(index) {
            case 0:
                loader.source = "./pages/FirstPage.qml"
                break;
            case 1:
                loader.source = "./pages/ConfigPage.qml"
                break;
            case 2:
                loader.source = "./pages/InfoPage.qml"
                break;
            default:
                loader.source = "./pages/FirstPage.qml"
                break;
            }
        }
    }

    Connections {
        target: app
        function onQmlMessageUpdate(message) {
            msgDialog.message = message.text
            msgDialog.open()
        }
    }

    Component.onCompleted: app.start()
}
