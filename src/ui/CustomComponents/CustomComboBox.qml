import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import "../defines.js" as Defines

ComboBox {
    id: control
    padding: 5
    hoverEnabled: true
    height: Defines.TOOLBAR_HEIGHT

    property alias textItem: text_item

    indicator: Label {
        x: control.width - width - 5
        y: control.topPadding + (control.availableHeight - height) / 2

        text: FontAwesome.caretDown
        font.family: FontAwesome.familySolid
        font.pixelSize: parent.height - 28
    }

    contentItem: Label {
        id: text_item
        leftPadding: 5
        rightPadding: control.indicator.width

        text: control.displayText
        font.pixelSize: parent.height - 28
        color: control.pressed ? palette.mid : (control.hovered ? palette.light : palette.buttonText)
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    delegate: ItemDelegate {
        hoverEnabled: true
        height: Defines.TOOLBAR_HEIGHT

        anchors.left: parent.left
        anchors.right: parent.right

        contentItem: Label {
            text: modelData
            color: pressed ? palette.mid : (hovered ? palette.light : palette.buttonText)

            font.pointSize: 12

            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter

            clip: true
            elide: Text.ElideRight
        }

        background: Rectangle {
            color: palette.dark
            border.color: palette.button
            border.width: hovered ? 1 : 0
        }
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            id: popup_content
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator {}
        }

        background: Rectangle {
            color: palette.base
            border.color: palette.alternateBase
            border.width: 1
        }
    }

    background: Rectangle {
        color: palette.dark
        border.color: palette.button
        border.width: 1
    }
}
