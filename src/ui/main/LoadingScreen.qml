import QtQuick 2.9
import QtQuick.Controls 2.2

Popup {
    id: splash
    width: parent.width
    height: parent.height

    background: Rectangle {
        color: "#222222"
    }

    visible: audio.status !== Loader.Ready

    Column {
        anchors.centerIn: parent
        spacing: 5

        Image {
            source: "/splash.jpg"
        }

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            text: qsTr("Loading ...")
            color: "white"
        }
    }
}
