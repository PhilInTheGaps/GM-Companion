import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import "../../../.."

Rectangle {
    id: root

    property string icon: FontAwesome.globe
    property font iconFont: FontAwesome.fontSolid
    property string placeholderText: "URL"
    property string examples: ""

    signal clicked(string url)

    anchors.right: parent.right
    anchors.left: parent.left

    height: Sizes.toolbarHeight
    border.color: Colors.border
    border.width: 1
    color: palette.window

    ToolTip.text: qsTr("Examples:\n") + root.examples
    ToolTip.visible: mouse_area.containsMouse

    Label {
        id: type_icon
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: height

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: root.icon
        font: root.iconFont

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            hoverEnabled: true
        }
    }

    TextField {
        id: text_field
        anchors.left: type_icon.right
        anchors.right: add_button.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10

        selectByMouse: true
        placeholderText: root.placeholderText
    }

    CustomToolBarButton {
        id: add_button
        anchors.right: parent.right
        anchors.rightMargin: 10

        iconText: FontAwesome.plus

        onClicked: {
            root.clicked(text_field.text);
            text_field.clear();
        }
    }
}
