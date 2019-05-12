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

    scale: 1 / image.scale

    Rectangle {
        width: marker_text.width
        height: marker_text.height

        x: -width / 2
        y: -height

        color: "transparent"

        Text {
            id: marker_text
            font.family: FontAwesome.familySolid
            text: icon
            anchors.centerIn: parent
            style: Text.Outline

            color: root.color
            styleColor: "black"

            font.pointSize: 30
        }

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            hoverEnabled: true

            drag.target: root
            drag.smoothed: false
            preventStealing: true

            onClicked: {
                left_swipe_view.currentIndex = 2
                marker_name.text = root.name
                marker_description.text = root.description
                marker_icon.text = root.icon
                marker_icon.color = root.color

                map_tool.markerIndex = root.markerIndex
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
                root.showDelete = true
            }
        }
    }

    Label {
        text: root.name
        font.pointSize: 14
        color: "white"
        padding: 5

        visible: mouse_area.containsMouse

        x: -width / 2
        y: 10
        z: 100

        background: Rectangle {
            color: "black"
            opacity: 0.7
        }
    }

    // Dialog for deletion
    Rectangle {
        color: color_scheme.secondaryBackgroundColor
        width: 150
        height: 150
        z: 150
        y: -(height / 2)
        x: 25

        visible: root.showDelete

        Column {
            id: dialog_column
            anchors.fill: parent
            anchors.margins: 10
            spacing: 5

            Text {
                text: qsTr("Delete?")
                font.pointSize: 12
                color: color_scheme.textColor
            }

            Button {
                text: qsTr("Yes")
                anchors.left: parent.left
                anchors.right: parent.right

                onClicked: {
                    root.showDelete = false
                    map_tool.deleteMarker(root.markerIndex)
                }
            }

            Button {
                text: qsTr("No")
                anchors.left: parent.left
                anchors.right: parent.right

                onClicked: root.showDelete = false
            }
        }
    }
}
