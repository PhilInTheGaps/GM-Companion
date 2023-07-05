import QtQuick
import QtQuick.Controls
import QtQuick.Effects

Button {
    id: root

    property int sides: 20
    property string imageSource: "/icons/dice/d" + sides + ".png"
    property string toolTipText: "D" + sides
    property bool isCurrentType: false

    height: parent.height - parent.padding * 2
    width: height

    background: Item {}

    Image {
        id: image
        source: root.imageSource

        anchors.fill: parent
        sourceSize.height: height
        sourceSize.width: width
    }

    MultiEffect {
        anchors.fill: image
        source: image
        colorization: 1
        colorizationColor: parent.pressed ? "grey" : isCurrentType ? "tomato" : "white"
    }

    ToolTip.text: toolTipText
    ToolTip.visible: hovered
    hoverEnabled: true
}
