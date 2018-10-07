import QtQuick 2.0
import QtQuick.Controls 2.2

ListElement {

    property var name

    MouseArea {

        ToolTip {
            id: tool_tip
            text: name
        }

        onEntered: tool_tip.visible = true

        onExited: tool_tip.visible = false
    }

    Text {
        text: name
    }
}
