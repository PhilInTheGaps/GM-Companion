import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."

Item {
    id: root

    required property MapMarkerEditor markerEditor

    signal back
    signal openDeleteDialog

    Column {
        id: marker_menu_column
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        // Back
        Item {
            height: Sizes.toolbarHeight
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5

            CustomToolBarButton {
                id: back_button
                iconText: FontAwesome.chevronLeft
                onClicked: root.back()
                anchors.margins: 7
            }

            Label {
                text: qsTr("Marker List")
                font.bold: true
                font.pointSize: 16
                verticalAlignment: Text.AlignVCenter

                anchors.left: back_button.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                elide: Label.ElideRight

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.back()
                }
            }
        }

        // Name and Icon
        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 10
            anchors.rightMargin: 0
            height: Sizes.toolbarHeight

            Label {
                id: marker_icon

                anchors.top: parent.top
                anchors.bottom: parent.bottom

                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                font.pointSize: 14
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                text: MapTool.currentMarker ? MapTool.currentMarker.icon : ""
                color: MapTool.currentMarker ? MapTool.currentMarker.color : ""
            }

            Label {
                id: marker_name
                font.pointSize: 14

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: marker_icon.right
                anchors.right: edit_button.left
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                text: MapTool.currentMarker ? MapTool.currentMarker.name : ""
            }

            CustomToolBarButton {
                id: edit_button
                iconText: FontAwesome.pen
                anchors.right: delete_button.left
                anchors.margins: 10
                pointSize: 14

                onClicked: {
                    root.markerEditor.markerName = marker_name.text;
                    root.markerEditor.markerDescription = marker_description.text;
                    root.markerEditor.markerIcon = marker_icon.text;
                    root.markerEditor.markerColor = marker_icon.color;
                    root.markerEditor.open();
                }
            }

            CustomToolBarButton {
                id: delete_button
                iconText: FontAwesome.trash
                anchors.right: parent.right
                anchors.margins: 10
                pointSize: 14

                onClicked: {
                    root.openDeleteDialog();
                }
            }
        }
    }

    // Description
    ScrollView {
        id: description_scroll_view
        anchors.top: marker_menu_column.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        anchors.rightMargin: 0

        contentWidth: -1
        clip: true

        ScrollBar.vertical: ScrollBar {
            anchors.right: parent.right
            height: description_scroll_view.availableHeight
            active: description_scroll_view.ScrollBar.horizontal.active
            visible: description_scroll_view.contentHeight > description_scroll_view.height
        }

        TextEdit {
            id: marker_description
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 10
            text: MapTool.currentMarker ? MapTool.currentMarker.description : ""

            font.pointSize: 11
            selectByMouse: true
            wrapMode: Text.WordWrap
            readOnly: true

            color: palette.text
            selectionColor: palette.highlight
            selectedTextColor: palette.highlightedText
        }
    }
}
