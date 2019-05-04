import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../../components"

CustomButton {
    anchors.left: parent.left
    anchors.right: parent.right

    height: color_scheme.toolbarHeight - 10
    color: "transparent"
    darkBackground: false
    textColor: color_scheme.textColor
    text.anchors.margins: 0
    text.font.pointSize: 12
}
