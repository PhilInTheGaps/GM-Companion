import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

import "../../../components"

Column {
    topPadding: 5
    spacing: 10

    // Image, Name ...
    Row {
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 10

        Rectangle {
            width: avatar_image.width + 10
            height: avatar_image.height + 10
            anchors.verticalCenter: parent.verticalCenter
            radius: width * 0.5
            color: "grey"

            Image {
                id: avatar_image
                height: 140
                width: 140
                sourceSize.width: width
                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                anchors.centerIn: parent

                source: character_dsa5_viewer.image
                visible: false
            }

            OpacityMask {
                anchors.fill: avatar_image
                source: avatar_image
                maskSource: Rectangle {
                    width: avatar_image.width
                    height: avatar_image.height
                    radius: width * 0.5
                    visible: false
                }
            }
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 10

            Label {
                text: character_dsa5_viewer.profileOverview[0]
                font.pointSize: 25
                font.bold: true
                color: color_scheme.textColor
            }

            Row {
                spacing: 10

                Repeater {
                    model: 4

                    Label {
                        text: character_dsa5_viewer.profileOverview[index + 1]
                        color: color_scheme.textColor
                    }
                }
            }

            Row {
                spacing: 10

                Repeater {
                    model: 2

                    Label {
                        text: character_dsa5_viewer.profileOverview[index + 5]
                        color: color_scheme.textColor
                    }
                }
            }
        }
    }

    Label {
        text: qsTr("Persönliche Daten")
        font.bold: true
        color: color_scheme.textColor
        font.pointSize: 12
    }

    Grid {
        id: overview_grid
        anchors.left: parent.left
        columns: 5
        columnSpacing: 50
        rowSpacing: 10

        property var titles: [qsTr("Familie"), qsTr("Geburtsort"), qsTr(
                "Geburtsdatum"), qsTr("Alter"), qsTr("Haarfarbe"), qsTr(
                "Augenfarbe"), qsTr("Körpergröße"), qsTr("Gewicht"), qsTr(
                "Titel"), qsTr("Sozialstatus")]

        Repeater {
            model: 10

            Column {
                spacing: 5

                Label {
                    text: overview_grid.titles[index]
                    color: color_scheme.textColor
                }

                Label {
                    text: character_dsa5_viewer.profileOverview[index + 7]
                    width: 160
                    height: 35
                    elide: Text.ElideRight
                    color: color_scheme.textColor
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                    leftPadding: 10
                    rightPadding: 10

                    background: Rectangle {
                        color: "transparent"
                        border.width: 1
                        border.color: "grey"
                    }
                }
            }
        }
    }

    Row {
        id: overview_row
        spacing: 50

        property var titles: [qsTr("Charakteristika"), qsTr("Sonstiges"), qsTr(
                "Ortskenntnis durch Kultur")]

        Repeater {
            model: 3

            Column {
                spacing: 5

                Label {
                    text: overview_row.titles[index]
                    color: color_scheme.textColor
                }

                Label {
                    text: character_dsa5_viewer.profileOverview[index + 17]
                    width: 300
                    height: 35
                    elide: Text.ElideRight
                    color: color_scheme.textColor
                    verticalAlignment: Text.AlignVCenter
                    padding: 5
                    leftPadding: 10
                    rightPadding: 10

                    background: Rectangle {
                        color: "transparent"
                        border.width: 1
                        border.color: "grey"
                    }
                }
            }
        }
    }
}
