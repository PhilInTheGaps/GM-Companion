import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import common

Button {
    id: root

    property int sides: 20
    property string imageSource: "qrc:/resources/icons/dice/d" + sides + ".png"
    property string toolTipText: qsTr("D") + sides
    property bool isCurrentType: false

    width: height

    background: Item {
    }

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
        colorizationColor: parent.pressed ? "grey" : root.isCurrentType ? SettingsManager.colors.error : "white"
    }

    ToolTip.text: toolTipText
    ToolTip.visible: hovered
    hoverEnabled: true
}
