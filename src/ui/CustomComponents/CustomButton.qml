import QtQuick 2.6
import QtQuick.Controls 2.2
import FontAwesome 2.0
import "../defines.js" as Defines

Control {
    id: root

    property alias mainRow: main_row
    property alias textItem: text_item
    property alias iconItem: icon_item

    property string buttonText: ""
    property string iconText: ""
    property var iconFont: FontAwesome.familySolid
    property int pointSize: 12
    readonly property var textColor: mouse_area.pressed ? palette.mid : (mouse_area.containsMouse ? palette.light : palette.buttonText)
    property var iconColor: undefined
    property var backgroundColor: undefined
    property bool usesFixedWidth: true
    property bool enableBold: true
    property bool centering: false
    property bool transparentBackground: false

    signal clicked(string info)

    padding: 5
    height: Defines.TOOLBAR_HEIGHT
    anchors.left: undefined
    hoverEnabled: true

    contentItem: Row {
        id: main_row
        spacing: 10

        property int spacingCount: icon_item.visible
                                   && text_item.visible ? 1 : 0
        property int contentWidth: (icon_item.visible ? icon_item.width : 0)
                                   + (text_item.visible ? text_item.width : 0) + spacingCount

        padding: centering ? (width - contentWidth) / 2 : 0

        Label {
            id: icon_item
            visible: text !== ""
            text: root.iconText
            color: iconColor ? iconColor : root.textColor

            font.family: root.iconFont
            font.bold: enableBold && mouse_area.containsMouse
            font.pointSize: root.pointSize

            width: height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            id: text_item
            visible: text !== ""
            text: root.buttonText
            color: root.textColor

            font.bold: enableBold && mouse_area.containsMouse

            font.pointSize: root.pointSize
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter

            width: usesFixedWidth ? main_row.width - x : implicitWidth

            clip: true
            elide: Text.ElideRight
        }
    }

    background: Rectangle {
        visible: !transparentBackground
        color: root.backgroundColor ? root.backgroundColor : palette.dark
        border.color: palette.button
        border.width: mouse_area.containsMouse ? 1 : 0
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked(parent.text)
    }
}
