import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Dialog {
    id: root

    property string markerName: "Placeholder"
    property string markerDescription: "Placeholder"
    property string markerColor: "red"
    property string markerIcon: FontAwesome.mapMarkerAlt

    title: qsTr("Edit Marker")
    modal: true

    standardButtons: Dialog.Cancel | Dialog.Save

    onAccepted: {
        map_tool.setMarkerProperties(marker_name_field.text,
                                     marker_description_edit.text,
                                     icon_text.text, color_field.text)
    }

    onVisibleChanged: {
        if (visible) {
            marker_name_field.text = markerName
            marker_description_edit.text = markerDescription
            icon_text.text = markerIcon
            color_field.text = markerColor
        }
    }

    contentItem: Row {
        spacing: 10

        Column {
            id: left_column
            spacing: 10
            width: 400

            CustomTextField {
                id: marker_name_field
                text: root.markerName
                font.pointSize: 14
                anchors.left: parent.left
                anchors.right: parent.right
                selectByMouse: true
            }

            CustomTextEdit {
                id: marker_description_edit
                text: root.markerDescription
                anchors.left: parent.left
                anchors.right: parent.right
                height: 300
            }
        }

        Column {
            id: mid_column
            spacing: 10
            width: 160

            Label {
                text: qsTr("Icon")
            }

            Label {
                id: icon_text
                text: root.markerIcon
                font.family: FontAwesome.familySolid
                font.pixelSize: color_field.height
                color: color_field.text
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Grid {
                anchors.left: parent.left
                anchors.right: parent.right

                spacing: 5
                columns: 4

                Repeater {
                    model: [FontAwesome.mapMarkerAlt, FontAwesome.mapMarker, FontAwesome.anchor, FontAwesome.rocket, FontAwesome.plane, FontAwesome.parking, FontAwesome.bed, FontAwesome.beer, FontAwesome.book, FontAwesome.building, FontAwesome.suitcase, FontAwesome.crosshairs, FontAwesome.flag, FontAwesome.flask, FontAwesome.home, FontAwesome.industry, FontAwesome.hospital, FontAwesome.fireExtinguisher, FontAwesome.gavel, FontAwesome.landmark, FontAwesome.shoppingBasket, FontAwesome.subway, FontAwesome.tree, FontAwesome.trophy]

                    Rectangle {
                        width: (parent.width - parent.spacing * 3) / 4
                        height: width

                        Label {
                            text: modelData
                            font.family: FontAwesome.familySolid
                            font.pixelSize: parent.height - 10
                            anchors.centerIn: parent
                        }

                        color: "transparent"
                        border.width: icon_mouse_area.containsMouse ? 2 : 0
                        border.color: palette.button

                        MouseArea {
                            id: icon_mouse_area
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                icon_text.text = modelData
                            }
                        }
                    }
                }
            }
        }

        Column {
            id: right_column
            spacing: 10
            width: 160

            Label {
                text: qsTr("Color")
            }

            CustomTextField {
                id: color_field
                text: root.markerColor
                font.pointSize: 12
                anchors.left: parent.left
                anchors.right: parent.right
                selectByMouse: true
            }

            Grid {
                anchors.left: parent.left
                anchors.right: parent.right

                spacing: 5
                columns: 4

                Repeater {
                    model: ["orange", "red", "crimson", "darkred", "blueviolet", "blue", "cornflowerblue", "dodgerblue", "deepskyblue", "cadetblue", "green", "limegreen", "yellowgreen", "chartreuse", "gold", "yellow", "khaki", "moccasin", "darkslategray", "slategrey", "grey", "silver", "lightsteelblue", "lightgrey"]

                    Rectangle {
                        width: (parent.width - parent.spacing * 3) / 4
                        height: width

                        color: modelData
                        border.width: color_mouse_area.containsMouse ? 2 : 0
                        border.color: palette.button

                        MouseArea {
                            id: color_mouse_area
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                color_field.text = modelData
                            }
                        }
                    }
                }
            }
        }
    }
}
