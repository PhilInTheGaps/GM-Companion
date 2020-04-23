import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../defines.js" as Defines
import "./maps"

Item {
    id: maps_page

    Connections {
        target: map_tool

        onMapIndexChanged: {
            marker_menu.visible = map_tool.currentMap ? map_tool.currentMap.hasMarkers : false
        }

        onMarkerIndexChanged: {
            if (map_tool.markerIndex > -1) {
                marker_menu.openMarkerDetails()
            } else {
                marker_menu.openMarkerList()
            }
        }
    }

    MapList {
        id: map_list
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 200
    }

    MapViewer {
        id: map_viewer
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: map_list.visible ? map_list.right : parent.left
        anchors.right: marker_menu.visible ? marker_menu.left : parent.right

        onToMarkerList: {
            marker_menu.visible = true
            marker_menu.openMarkerList()
        }

        onToMarkerDetails: {
            marker_menu.visible = true
            marker_menu.openMarkerDetails()
        }

        onMarkerMenuToggled: {
            marker_menu.visible = !marker_menu.visible
        }
    }

    MapMarkerMenu {
        id: marker_menu
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 200
        visible: false

        onOpenDeleteDialog: map_viewer.openDeleteDialog()
    }

    MapMarkerEditor {
        id: marker_editor
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
    }

    MapMarkerLabel {
        id: marker_name_label
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
