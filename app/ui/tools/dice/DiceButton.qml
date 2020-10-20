import QtQuick 2.9
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

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

    ColorOverlay {
        anchors.fill: image
        source: image
        color: parent.pressed ? "grey" : isCurrentType ? "tomato" : "white"
    }

    ToolTip.text: toolTipText
    ToolTip.visible: hovered
    hoverEnabled: true
}
