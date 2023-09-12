import QtQuick
import src
import "./maps"

Item {
    id: maps_page

    Component.onCompleted: MapTool.loadData()

    Connections {
        target: MapTool

        function onMapIndexChanged() {
            marker_menu.visible = MapTool.currentMap ? MapTool.currentMap.hasMarkers : false
        }

        function onMarkerIndexChanged() {
            if (MapTool.markerIndex > -1) {
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

        markerNameLabel: marker_name_label

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

        markerEditor: marker_editor

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
