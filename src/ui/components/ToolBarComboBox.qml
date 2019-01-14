import QtQuick 2.9
import QtQuick.Controls 2.3

import FontAwesome 2.0

ComboBox {
    id: control
    hoverEnabled: true

    delegate: ItemDelegate {
        width: control.width

        contentItem: Text {
            text: modelData
            color: "black"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        highlighted: control.highlightedIndex === index
    }

    indicator: Text {
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        text: FontAwesome.caretDown
        font.family: FontAwesome.familySolid
        font.pixelSize: parent.height - 20
        color: "white"
    }

    contentItem: Text {
        leftPadding: 5
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font.pixelSize: parent.height - 20
        color: control.pressed ? "grey" : control.hovered ? "lightgrey" : "white"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 40
        color: "transparent"
        border.width: 0
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator {
            }
        }

        background: Rectangle {
            border.color: color_scheme.menuColor
        }
    }
}
