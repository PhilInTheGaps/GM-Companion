import QtQuick 2.9
import QtQuick.Controls 2.2

Popup {
    id: splash

    property real progress: 0
    property bool showImage: true

    width: parent.width
    height: parent.height

    visible: true

    Column {
        anchors.centerIn: parent
        spacing: 10

        width: showImage ? image.width : parent.width / 2

        Image {
            id: image
            visible: showImage
            source: "/splash.jpg"
        }

        ProgressBar {
            id: progress_bar
            value: splash.progress
            anchors.left: parent.left
            anchors.right: parent.right

            background: Rectangle {
                implicitHeight: 10
                color: palette.dark
                radius: 3
                border.color: palette.button
                border.width: 1
            }

            contentItem: Item {
                implicitHeight: 8

                Rectangle {
                    width: progress_bar.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: palette.alternateBase
                }
            }
        }

        Text {
            text: qsTr("Loading ...")
            color: "white"
        }
    }
}
