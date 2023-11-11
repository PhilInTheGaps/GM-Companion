import QtQuick
import QtQuick.Controls
import IconFonts
import src

Item {
    id: root

    required property Label markerNameLabel

    property string name: qsTr("New Location")
    property string description
    property string color: "red"
    property string icon: FontAwesome.locationDot
    property int markerIndex: -1
    property bool showDelete: false

    signal markerSelected
    signal openDeleteDialog

    rotation: -parent.parent.rotation

    Rectangle {
        width: marker_text.width
        height: marker_text.height

        x: -width / 2
        y: -height

        color: "transparent"

        Label {
            id: marker_text
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            text: root.icon
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
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onContainsMouseChanged: {
                if (containsMouse) {
                    root.markerNameLabel.text = root.name;
                }
                root.markerNameLabel.visible = containsMouse;
            }

            onClicked: mouse => {
                MapTool.markerIndex = root.markerIndex;
                if (mouse.button === Qt.LeftButton) {
                    root.markerSelected();
                } else if (mouse.button === Qt.RightButton) {
                    root.openDeleteDialog();
                }
            }

            drag.onActiveChanged: {
                if (!drag.active)
                    MapTool.setMarkerPosition(root.markerIndex, root.x, root.y);
            }
        }
    }
}
