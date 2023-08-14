import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts
import "../defines.js" as Defines

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
