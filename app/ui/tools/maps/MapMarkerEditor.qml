pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."

Dialog {
    id: root

    property string markerName: "Placeholder"
    property string markerDescription: "Placeholder"
    property color markerColor: "red"
    property string markerIcon: FontAwesome.locationDot

    title: qsTr("Edit Marker")
    modal: true

    standardButtons: Dialog.Cancel | Dialog.Save

    onOpened: {
        marker_name_field.forceActiveFocus();
        marker_name_field.selectAll();
    }

    onAccepted: {
        MapTool.setMarkerProperties(marker_name_field.text, marker_description_edit.text, icon_text.text, color_field.text);
    }

    onVisibleChanged: {
        if (visible) {
            marker_name_field.text = markerName;
            marker_description_edit.text = markerDescription;
            icon_text.text = markerIcon;
            color_field.text = markerColor;
        }
    }

    contentItem: Row {
        spacing: 10

        Column {
            id: left_column
            spacing: 10
            width: 400

            TextField {
                id: marker_name_field
                text: root.markerName
                font.pointSize: 14
                anchors.left: parent.left
                anchors.right: parent.right
                selectByMouse: true

                placeholderText: qsTr("Enter name ...")
            }

            CustomTextEdit {
                id: marker_description_edit
                text: root.markerDescription
                anchors.left: parent.left
                anchors.right: parent.right
                height: 300
                placeholderText: qsTr("Enter description ...")
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
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                font.pixelSize: color_field.height
                color: color_field.text
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Grid {
                id: icon_grid
                anchors.left: parent.left
                anchors.right: parent.right

                spacing: 5
                columns: 4

                Repeater {
                    model: [FontAwesome.locationDot, FontAwesome.locationPin, FontAwesome.anchor, FontAwesome.rocket, FontAwesome.plane, FontAwesome.squareParking, FontAwesome.bed, FontAwesome.beerMugEmpty, FontAwesome.book, FontAwesome.building, FontAwesome.suitcase, FontAwesome.crosshairs, FontAwesome.flag, FontAwesome.flask, FontAwesome.house, FontAwesome.industry, FontAwesome.hospital, FontAwesome.fireExtinguisher, FontAwesome.gavel, FontAwesome.landmark, FontAwesome.basketShopping, FontAwesome.train, FontAwesome.tree, FontAwesome.trophy]

                    Rectangle {
                        id: icon_delegate
                        required property string modelData

                        width: (parent.width - icon_grid.spacing * 3) / 4
                        height: width

                        Label {
                            text: icon_delegate.modelData
                            font.family: FontAwesome.fontSolid.family
                            font.styleName: FontAwesome.fontSolid.styleName
                            font.pixelSize: parent.height - 10
                            anchors.centerIn: parent
                        }

                        color: "transparent"
                        border.width: icon_mouse_area.containsMouse ? 2 : 0
                        border.color: icon_mouse_area.pressed ? Colors.buttonPressed : palette.button

                        MouseArea {
                            id: icon_mouse_area
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                icon_text.text = icon_delegate.modelData;
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

            TextField {
                id: color_field
                text: "%1".arg(root.markerColor)
                font.pointSize: 12
                anchors.left: parent.left
                anchors.right: parent.right
                selectByMouse: true
                placeholderText: qsTr("Enter marker description ...")
            }

            Grid {
                id: color_grid
                anchors.left: parent.left
                anchors.right: parent.right

                spacing: 5
                columns: 4

                Repeater {
                    model: ["orange", "red", "crimson", "darkred", "blueviolet", "blue", "cornflowerblue", "dodgerblue", "deepskyblue", "cadetblue", "green", "limegreen", "yellowgreen", "chartreuse", "gold", "yellow", "khaki", "moccasin", "darkslategray", "slategrey", "grey", "silver", "lightsteelblue", "lightgrey"]

                    CustomColorButton {
                        required property string modelData
                        color: modelData

                        width: (parent.width - color_grid.spacing * 3) / 4
                        height: width

                        onClicked: {
                            color_field.text = modelData;
                        }
                    }
                }
            }
        }
    }
}
