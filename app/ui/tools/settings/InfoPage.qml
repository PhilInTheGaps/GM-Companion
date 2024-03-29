import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts

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
                        "iconFont": FontAwesome.fontSolid,
                        "url": "https://gm-companion.github.io/docs/"
                    }, {
                        "name": qsTr("Report a Bug"),
                        "icon": FontAwesome.bug,
                        "iconFont": FontAwesome.fontSolid,
                        "url": "https://github.com/PhilInTheGaps/GM-Companion/issues"
                    }, {
                        "name": qsTr("Older Versions"),
                        "icon": FontAwesome.download,
                        "iconFont": FontAwesome.fontSolid,
                        "url": "https://github.com/PhilInTheGaps/GM-Companion/releases"
                    }, {
                        "name": qsTr("View on GitHub"),
                        "icon": FontAwesome.github,
                        "iconFont": FontAwesome.fontBrands,
                        "url": "https://github.com/PhilInTheGaps/GM-Companion"
                    }, {
                        "name": qsTr("Changelog"),
                        "icon": FontAwesome.clipboardList,
                        "iconFont": FontAwesome.fontSolid,
                        "url": "https://gm-companion.github.io/docs/changelog/"
                    }]

                CustomButton {
                    required property var modelData

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
            text: qsTr(
                      "Credits - Open Source projects and other resources used by this application")
            font.pointSize: 12
            anchors.top: parent.top
            anchors.left: parent.left
            wrapMode: Label.WrapAtWordBoundaryOrAnywhere
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

            ScrollBar.vertical: ScrollBar {
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
                    title: "CMake Modules"
                    description: qsTr("Additional CMake functionality")
                    license: [["BSL-1.0", "https://github.com/bilke/cmake-modules/blob/master/LICENSE_1_0.txt"], ["BSD", "https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake"], ["MIT", "https://github.com/bilke/cmake-modules/blob/master/cotire-license"]]
                    links: [["bilke/cmake-modules", "https://github.com/bilke/cmake-modules"]]
                }

                CreditsItem {
                    title: qsTr("Dice and effect icons")
                    description: qsTr("Icons by Skoll, Delapouite, Lorc and Cathelineau")
                    license: [["CC BY 3.0", "https://creativecommons.org/licenses/by/3.0/"]]
                    links: [["Skoll", "https://game-icons.net"], ["Delapouite", "https://delapouite.com"], ["Lorc", "https://lorcblog.blogspot.com"], ["Cathelineau", "https://game-icons.net"]]
                }

                CreditsItem {
                    title: "QML IconFonts"
                    description: qsTr("Use icon fonts in QML")
                    license: [["Font Awesome: SIL OFL 1.1", "https://scripts.sil.org/OFL"], ["Library: GPLv3", "https://www.gnu.org/licenses/gpl-3.0.en.html"]]
                    links: [[qsTr("qml-icon-fonts"), "https://github.com/PhilInTheGaps/qml-icon-fonts"], ["FontAwesome", "https://fontawesome.com"]]
                }

                CreditsItem {
                    title: "http-status-codes-cpp"
                    description: qsTr("HTTP Status Codes and Reason Phrases for C, C++ and Qt")
                    license: [["CC0 1.0 Universal", "https://github.com/j-ulrich/http-status-codes-cpp/blob/main/LICENSE"]]
                    links: [["j-ulrich/http-status-codes-cpp", "https://github.com/j-ulrich/http-status-codes-cpp"]]
                }

                CreditsItem {
                    title: "Librespot"
                    description: qsTr("Open Source Spotify client library")
                    license: [["MIT", "https://github.com/librespot-org/librespot/blob/dev/LICENSE"]]
                    links: [["librespot-org/librespot", "https://github.com/librespot-org/librespot"]]
                }

                CreditsItem {
                    title: "Names"
                    description: qsTr("Various resources used for the name generator")
                    license: [["CC BY 4.0", "https://github.com/fivethirtyeight/data/blob/master/LICENSE"], ["MIT", "https://github.com/faker-ruby/faker/blob/master/License.txt"]]
                    links: [["Unisex Names", "https://github.com/fivethirtyeight/data/tree/master/unisex-names"], ["Faker", "https://github.com/faker-ruby/faker"]]
                }

                CreditsItem {
                    title: "O2 OAuth2 Library"
                    description: qsTr("A library for OAuth2 authentication by pipacs.")
                    license: [["BSD 2-Clause", "https://github.com/pipacs/o2/blob/master/LICENSE"]]
                    links: [["GitHub", "https://github.com/pipacs/o2"], ["pipacs", "https://github.com/pipacs"]]
                }

                CreditsItem {
                    title: "Qt Project"
                    description: qsTr("A C++ toolkit for cross-platform application development.")
                    license: [["GPL v3", "https://www.gnu.org/licenses/gpl-3.0.en.html"]]
                    links: [["qt.io", "https://qt.io"]]
                }

                CreditsItem {
                    title: "Qt-PropertyHelper"
                    description: qsTr("Makros for reducing boilerplate code related to Qt properties")
                    links: [["tackelua/Qt-PropertyHelper", "https://github.com/tackelua/Qt-PropertyHelper"], ["Original Gist", "https://gist.github.com/Rolias/48d453a0490d36090193"]]
                }

                CreditsItem {
                    title: "QuaZip"
                    description: qsTr("A Qt/C++ library for working with Zip files.")
                    license: [["LGPL v2.1", "https://github.com/stachenov/quazip/blob/master/COPYING"]]
                    links: [["GitHub", "https://github.com/stachenov/quazip"]]
                }

                CreditsItem {
                    title: "Sentry Native SDK"
                    description: qsTr("Crash report SDK")
                    license: [["MIT", "https://github.com/getsentry/sentry-native/blob/master/LICENSE"]]
                    links: [["getsentry/sentry-native", "https://github.com/getsentry/sentry-native"]]
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
