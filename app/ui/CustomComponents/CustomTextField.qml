import QtQuick 2.9
import QtQuick.Controls 2.2
import "../defines.js" as Defines

TextField {
    id: root

    property string backgroundColor: "transparent"

    selectByMouse: true

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: Defines.TOOLBAR_HEIGHT
        color: backgroundColor
        border.color: root.enabled ? palette.button : palette.dark
    }
}
