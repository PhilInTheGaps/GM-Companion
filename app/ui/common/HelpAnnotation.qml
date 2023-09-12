import QtQuick
import QtQuick.Controls
import IconFonts

Label {
    id: root
    property string helpText: ""

    text: FontAwesome.circleInfo
    font.family: FontAwesome.fontSolid.family
    font.styleName: FontAwesome.fontSolid.styleName
    verticalAlignment: Text.AlignVCenter

    ToolTip {
        text: root.helpText
        visible: mouse_area.containsMouse
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true
    }
}
