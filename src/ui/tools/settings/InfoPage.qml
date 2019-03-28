import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: root

    Column {
        id: left_column
        spacing: 5
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        width: 200

        property var button_names: [qsTr("Open Wiki"), qsTr(
                "Report a Bug"), qsTr("Download older Versions"), qsTr(
                "View on GitHub"), qsTr("Changelog"), qsTr("About")]
        property var button_urls: ["https://github.com/PhilInTheGaps/GM-Companion/wiki", "https://github.com/PhilInTheGaps/GM-Companion/issues", "https://github.com/PhilInTheGaps/GM-Companion/releases", "https://github.com/PhilInTheGaps/GM-Companion", "https://github.com/PhilInTheGaps/GM-Companion/wiki/changelog", "https://gm-companion.github.io/about.html"]

        Repeater {
            model: left_column.button_names

            Button {
                text: modelData
                anchors.left: parent.left
                anchors.right: parent.right

                onClicked: Qt.openUrlExternally(left_column.button_urls[index])
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Version: " + update_manager.getCurrentVersion()
            color: color_scheme.textColor
        }
    }

    Item {
        anchors.left: left_column.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5

        Label {
            id: credits_label
            text: qsTr("Credits")
            font.pointSize: 12
            anchors.top: parent.top
            anchors.left: parent.left
            color: color_scheme.textColor
        }

        ScrollView {
            anchors.top: credits_label.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10
            contentHeight: credits_column.implicitHeight
            contentWidth: -1
            clip: true

            Column {
                id: credits_column
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10

                CreditsItem {
                    title: "Qt Project"
                    description: qsTr("A C++ toolkit for cross-platform application development.")
                    links: [["qt.io", "https://qt.io"]]
                }

                CreditsItem {
                    title: qsTr("Dice and effect icons")
                    description: qsTr("Icons by Skoll, Delapouite, Lorc and Cathelineau under CC BY 3.0.")
                    links: [["Skoll", "https://game-icons.net"], ["Delapouite", "https://delapouite.com"], ["Lorc", "https://lorcblog.blogspot.com"], ["Cathelineau", "https://game-icons.net"], ["CC BY 3.0", "https://creativecommons.org/licenses/by/3.0"]]
                }

                CreditsItem {
                    title: qsTr("Music, Sound and Radio icons")
                    description: qsTr("Icons by Marius Masalar, Davin Martin Jr. and Alex Blăjan from Unsplash.")
                    links: [["Marius Masalar", "https://unsplash.com/@marius"], ["Davin Martin Jr.", "https://unsplash.com/@davidmartinjr"], ["Alex Blăjan", "https://unsplash.com/@alexb"], ["Unsplash", "https://unsplash.com"]]
                }

                CreditsItem {
                    title: "FontAwesome"
                    description: qsTr("A great icon library.")
                    links: [["fontawesome.com", "https://fontawesome.com"]]
                }

                CreditsItem {
                    title: "FontAwesome.pri"
                    description: qsTr("FontAwesome QML implementation. This project uses a fork of FontAwesome.pri by Ben Lau.")
                    links: [[qsTr("Original"), "https://github.com/benlau/fontawesome.pri"], [qsTr("Fork"), "https://github.com/PhilInTheGaps/fontawesome.pri"], ["Ben Lau", "https://github.com/benlau"]]
                }

                CreditsItem {
                    title: "O2 OAuth2 Library"
                    description: qsTr("A library for OAuth2 authentication by pipacs.")
                    links: [["GitHub", "https://github.com/pipacs/o2"], ["pipacs", "https://github.com/pipacs"]]
                }

                CreditsItem {
                    title: "Spotify Playlists"
                    description: qsTr("Awesome playlists by Brian Davis (Bezoing) and gentilpuck.")
                    links: [["Brian Davis", "https://open.spotify.com/user/bezoing?si=acN6RQebQTS2iZEUWDKRKA"], ["gentilpuck", "https://open.spotify.com/user/gentilpuck?si=nZpk5I8wTBuWdcBrrix_qg"]]
                }
            }
        }
    }
}
