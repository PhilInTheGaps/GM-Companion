import QtQuick 2.9

Item {
    id: root
    anchors.fill: parent

    signal openMarkerDetails

    Repeater {
        id: marker_repeater
        model: mapMarkerModel

        MapMarker {
            name: modelData.name
            description: modelData.description
            color: modelData.color
            icon: modelData.icon
            markerIndex: index

            scale: 1 / root.parent.scale

            x: modelData.x
            y: modelData.y

            onMarkerSelected: openMarkerDetails()
        }
    }
}
