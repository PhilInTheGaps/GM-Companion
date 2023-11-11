pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import src

Item {
    id: root

    required property Label markerNameLabel

    signal openMarkerDetails
    signal openDeleteDialog

    anchors.fill: parent

    Repeater {
        id: marker_repeater
        model: MapTool.markerModel

        MapMarkerIcon {
            required property MapMarker modelData
            required property int index

            name: modelData.name
            description: modelData.description
            color: modelData.color
            icon: modelData.icon
            markerIndex: index

            markerNameLabel: root.markerNameLabel
            onOpenDeleteDialog: root.openDeleteDialog()

            scale: 1 / root.parent.scale

            x: modelData.x
            y: modelData.y

            onMarkerSelected: root.openMarkerDetails()
        }
    }
}
