import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.impl 2.2
import QtQuick.Templates 2.2 as T
import FontAwesome 2.0
import "../colors.js" as Colors
import "../sizes.js" as Sizes

T.ComboBox {
    id: control

    hoverEnabled: true
    font.pixelSize: height - 28

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0, Math.max(
                            contentItem.implicitHeight,
                            indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    leftPadding: padding + (!control.mirrored || !indicator
                            || !indicator.visible ? 0 : indicator.width + spacing)
    rightPadding: padding + (control.mirrored || !indicator
                             || !indicator.visible ? 0 : indicator.width + spacing)

    delegate: ItemDelegate {
        width: parent.width
        text: control.textRole ? (Array.isArray(
                                      control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
        font.weight: control.currentIndex === index ? Font.DemiBold : Font.Normal
        highlighted: control.highlightedIndex === index
        hoverEnabled: control.hoverEnabled
    }

    indicator: Label {
        readonly property int offset: control.padding + width / 2
        x: control.mirrored ? offset : control.width - width - offset
        y: control.topPadding + (control.availableHeight - height) / 2

        text: FontAwesome.caretDown
        font.family: FontAwesome.familySolid
        font.pixelSize: parent.height - 28
        opacity: enabled && control.model && control.model.length > 0 ? 1 : 0.5
    }

    contentItem: TextField {
        leftPadding: !control.mirrored ? 12 : control.editable
                                         && activeFocus ? 3 : 1
        rightPadding: control.mirrored ? 12 : control.editable
                                         && activeFocus ? 3 : 1
        topPadding: 6 - control.padding
        bottomPadding: 6 - control.padding
        text: control.editable ? control.editText : control.displayText

        enabled: control.editable
        autoScroll: control.editable
        readOnly: control.down
        inputMethodHints: control.inputMethodHints
        validator: control.validator

        font: control.font
        color: !control.editable
               && control.visualFocus ? Colors.focus : Colors.text
        selectionColor: Colors.focus
        selectedTextColor: Colors.highlightedText
        verticalAlignment: Text.AlignVCenter
        opacity: control.enabled ? 1 : 0.3

        background: Rectangle {
            visible: control.editable && !control.flat
            border.width: parent && parent.activeFocus ? 2 : 1
            border.color: parent
                          && parent.activeFocus ? Colors.focus : Colors.border
            implicitHeight: 40
        }
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: Sizes.comboBoxHeight

        color: Colors.dark
        border.color: Colors.border
        border.width: 1
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        height: Math.min(contentItem.implicitHeight,
                         control.Window.height - topMargin - bottomMargin)
        topMargin: 5
        bottomMargin: 5
        padding: 0
        modal: control.model.length
        transformOrigin: Item.Top

        enter: Transition {
            // grow_fade_in
            NumberAnimation {
                property: "scale"
                from: 0.9
                to: 1.0
                easing.type: Easing.OutQuint
                duration: 220
            }
            NumberAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
                easing.type: Easing.OutCubic
                duration: 150
            }
        }

        exit: Transition {
            // shrink_fade_out
            NumberAnimation {
                property: "scale"
                from: 1.0
                to: 0.9
                easing.type: Easing.OutQuint
                duration: 220
            }
            NumberAnimation {
                property: "opacity"
                from: 1.0
                to: 0.0
                easing.type: Easing.OutCubic
                duration: 150
            }
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.highlightedIndex
            highlightRangeMode: ListView.ApplyRange
            highlightMoveDuration: 0

            Rectangle {
                z: 10
                width: parent.width
                height: parent.height
                color: "transparent"
                border.color: Colors.border
            }

            T.ScrollIndicator.vertical: ScrollIndicator {}
        }

        background: Rectangle {
            color: Colors.dark
            border.color: Colors.alternateBase
            border.width: 1
        }
    }
}
