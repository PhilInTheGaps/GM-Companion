import QtQuick
import QtQuick.Controls
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

    SplitView {
        id: split_view

        orientation: Qt.Horizontal
        anchors.fill: parent

        MapList {
            id: map_list
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: 200
        }

        MapViewer {
            id: map_viewer
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 250
            SplitView.fillWidth: true

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

            MapMarkerLabel {
                id: marker_name_label
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        MapMarkerMenu {
            id: marker_menu

            visible: false

            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: 250

            markerEditor: marker_editor

            onOpenDeleteDialog: map_viewer.openDeleteDialog()
        }
    }

    MapMarkerEditor {
        id: marker_editor
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
    }
}
