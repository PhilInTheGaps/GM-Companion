import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Item {
    id: root

    Rectangle {
        id: right_item
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 175

        color: palette.dark

        Column {
            anchors.fill: parent
            anchors.margins: 5

            Repeater {
                model: [{
                        "name": qsTr("Open Wiki"),
                        "icon": FontAwesome.book,
                        "iconFont": FontAwesome.familySolid,
                        "url": "https://gm-companion.github.io/documentation.html"
                    }, {
                        "name": qsTr("Report a Bug"),
                        "icon": FontAwesome.bug,
                        "iconFont": FontAwesome.familySolid,
                        "url": "https://github.com/PhilInTheGaps/GM-Companion/issues"
                    }, {
                        "name": qsTr("Older Versions"),
                        "icon": FontAwesome.download,
                        "iconFont": FontAwesome.familySolid,
                        "url": "https://github.com/PhilInTheGaps/GM-Companion/releases"
                    }, {
                        "name": qsTr("View on GitHub"),
                        "icon": FontAwesome.github,
                        "iconFont": FontAwesome.familyBrands,
                        "url": "https://github.com/PhilInTheGaps/GM-Companion"
                    }, {
                        "name": qsTr("Changelog"),
                        "icon": FontAwesome.clipboardList,
                        "iconFont": FontAwesome.familySolid,
                        "url": "https://gm-companion.github.io/documentation/changelog.html"
                    }]

                CustomButton {
                    buttonText: modelData.name
                    iconText: modelData.icon
                    iconFont: modelData.iconFont
                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: Qt.openUrlExternally(modelData.url)
                }
            }
        }
    }

    Item {
        id: mid_item
        anchors.left: parent.left
        anchors.right: right_item.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 10

        Label {
            id: credits_label
            text: qsTr("Credits")
            font.pointSize: 12
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Flickable {
            id: flickable
            anchors.top: credits_label.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10

            contentHeight: credits_column.implicitHeight
            contentWidth: -1
            interactive: true
            clip: true

            ScrollBar.vertical: CustomScrollBar {
                id: scroll_bar
                visible: flickable.contentHeight > flickable.height
            }

            Column {
                id: credits_column
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0
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
                    title: "FontAwesome.pri"
                    description: qsTr("FontAwesome QML implementation. This project uses a fork of FontAwesome.pri by Ben Lau.")
                    links: [[qsTr("Original"), "https://github.com/benlau/fontawesome.pri"], [qsTr("Fork"), "https://github.com/PhilInTheGaps/fontawesome.pri"], ["Ben Lau", "https://github.com/benlau"], ["FontAwesome", "https://fontawesome.com"]]
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
