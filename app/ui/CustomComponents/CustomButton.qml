import QtQuick
import QtQuick.Controls
import IconFonts

Control {
    id: root

    property alias mainRow: main_row
    property alias textItem: text_item
    property alias iconItem: icon_item
    property alias mouseArea: mouse_area
    property alias pressed: mouse_area.pressed

    // Content
    property string buttonText: ""
    property string iconText: ""
    property string toolTipText: ""
    property font iconFont: FontAwesome.fontSolid

    // Visual options
    property int pointSize: 12
    readonly property color textColor: mouse_area.pressed ? palette.mid : (mouse_area.containsMouse ? CCColors.focus : palette.text)
    property color iconColor: textColor
    property color backgroundColor: palette.dark
    property bool transparentBackground: false
    property int borderWidth: 1
    property bool forceBorderVisible: false

    // Layout
    property bool usesFixedWidth: true
    property bool enableBold: true
    property bool centering: false

    signal clicked(string info)
    signal rightClicked(string info)

    padding: 5
    height: CCSizes.customButtonHeight
    anchors.left: undefined
    hoverEnabled: true

    contentItem: Row {
        id: main_row
        spacing: 10

        property int spacingCount: icon_item.visible && text_item.visible ? 1 : 0
        property int contentWidth: (icon_item.visible ? icon_item.width : 0) + (text_item.visible ? text_item.width : 0) + spacingCount

        padding: root.centering ? (width - contentWidth) / 2 : 0

        Label {
            id: icon_item
            visible: text.length
            text: root.iconText
            color: root.iconColor
            opacity: root.enabled ? 1 : 0.7

            font.family: root.iconFont.family
            font.styleName: root.iconFont.styleName
            font.bold: root.enableBold && mouse_area.containsMouse
            font.pointSize: root.pointSize

            width: height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            id: text_item
            visible: text.length
            text: root.buttonText
            color: root.textColor
            opacity: root.enabled ? 1 : 0.7

            font.bold: root.enableBold && mouse_area.containsMouse

            font.pointSize: root.pointSize
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter

            width: root.usesFixedWidth ? main_row.width - x : implicitWidth

            clip: true
            elide: Text.ElideRight
        }
    }

    background: Rectangle {
        visible: !root.transparentBackground
        color: root.backgroundColor
        border.color: root.pressed ? CCColors.buttonPressed : palette.button
        border.width: mouse_area.containsMouse || root.forceBorderVisible ? root.borderWidth : 0
    }

    ToolTip.text: root.toolTipText
    ToolTip.visible: root.toolTipText.length && mouse_area.containsMouse
    ToolTip.delay: 500

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: function (mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.clicked(root.buttonText);
            } else if (mouse.button === Qt.RightButton) {
                root.rightClicked(root.buttonText);
            }
        }
    }
}
