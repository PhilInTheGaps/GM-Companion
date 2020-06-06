import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

Item {
    id: root
    property string name: qsTr("New Location")
    property string description
    property string color: "red"
    property string icon: FontAwesome.mapMarkerAlt
    property int markerIndex: -1
    property bool showDelete: false

    signal markerSelected

    rotation: -parent.parent.rotation

    Rectangle {
        width: marker_text.width
        height: marker_text.height

        x: -width / 2
        y: -height

        color: "transparent"

        Label {
            id: marker_text
            font.family: FontAwesome.familySolid
            text: icon
            anchors.centerIn: parent

            color: root.color
            style: Text.Outline
            styleColor: palette.dark

            font.pointSize: 30
        }

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            hoverEnabled: true

            drag.target: root
            drag.smoothed: false
            preventStealing: true

            onContainsMouseChanged: {
                if (containsMouse) {
                    marker_name_label.text = root.name
                }

                marker_name_label.visible = containsMouse
            }

            onClicked: {
                map_tool.markerIndex = root.markerIndex
                markerSelected()
            }

            drag.onActiveChanged: {
                if (!drag.active)
                    map_tool.setMarkerPosition(markerIndex, root.x, root.y)
            }
        }

        // For right-clicks
        MouseArea {
            id: mouse_area2
            anchors.fill: parent
            propagateComposedEvents: true
            acceptedButtons: Qt.RightButton

            onClicked: {
                map_tool.markerIndex = root.markerIndex
                marker_delete_rect.visible = true
            }
        }
    }
}
