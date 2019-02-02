import QtQuick 2.9
import QtQuick.Controls 2.2

ScrollView {
    contentWidth: -1

    Column {

        anchors.left: parent.left
        anchors.right: parent.right

        spacing: 5

        PathItem {
            path_type: qsTr("Audio Projects")
            setting: "audio"
            width: parent.width
        }

        PathItem {
            path_type: qsTr("Music")
            setting: "music"
            width: parent.width
        }

        PathItem {
            path_type: qsTr("Sound")
            setting: "sound"
            width: parent.width
        }

        PathItem {
            path_type: qsTr("Internet Radio Playlists")
            setting: "radio"
            width: parent.width
        }

        PathItem {
            path_type: qsTr("Maps")
            setting: "maps"
            width: parent.width
        }

        PathItem {
            path_type: qsTr("Characters")
            setting: "characters"
            width: parent.width
        }

        PathItem {
            path_type: qsTr("Notes")
            setting: "notes"
            width: parent.width
        }

        PathItem {
            path_type: qsTr("Shop Projects / Item Library")
            setting: "shop"
            width: parent.width
        }

        PathItem {
            path_type: qsTr("Resources")
            setting: "resources"
            width: parent.width
        }

        Rectangle {
            height: 10
            width: height
            color: "transparent"
        }
    }
}
