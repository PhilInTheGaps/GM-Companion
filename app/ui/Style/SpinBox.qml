import QtQuick
import QtQuick.Templates as T
import IconFonts

T.SpinBox {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + 2 * padding
                       + (up.indicator ? up.indicator.implicitWidth : 0)
                       + (down.indicator ? down.indicator.implicitWidth : 0))
    implicitHeight: Math.max(
                        contentItem.implicitHeight + topPadding + bottomPadding,
                        background ? background.implicitHeight : 0,
                        up.indicator ? up.indicator.implicitHeight : 0,
                        down.indicator ? down.indicator.implicitHeight : 0)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: 6
    leftPadding: padding + (control.mirrored ? (up.indicator ? up.indicator.width : 0) : (down.indicator ? down.indicator.width : 0))
    rightPadding: padding + (control.mirrored ? (down.indicator ? down.indicator.width : 0) : (up.indicator ? up.indicator.width : 0))
    hoverEnabled: true

    validator: IntValidator {
        // qmllint disable missing-property
        locale: control.locale.name
        // qmllint enable missing-property
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }

    contentItem: TextInput {
        z: 2
        text: control.displayText
        opacity: control.enabled ? 1 : 0.3

        font: control.font
        color: palette.text
        selectionColor: palette.highlight
        selectedTextColor: palette.highlightedText
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: control.inputMethodHints

        Rectangle {
            x: -6 - (control.down.indicator ? 1 : 0)
            y: -6
            width: control.width - (control.up.indicator ? control.up.indicator.width - 1 : 0)
                   - (control.down.indicator ? control.down.indicator.width - 1 : 0)
            height: control.height
            color: "transparent"
            border.color: control.activeFocus ? StyleColors.borderFocus : StyleColors.border
            border.width: control.activeFocus ? 2 : 1
        }
    }

    up.indicator: Rectangle {
        x: control.mirrored ? 0 : parent.width - width
        height: parent.height
        implicitWidth: height
        implicitHeight: StyleSizes.spinBoxHeight
        color: palette.dark
        border.color: StyleColors.border
        border.width: control.up.hovered ? 1 : 0

        Label {
            text: FontAwesome.plus
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            anchors.centerIn: parent
            color: enabled ? palette.text : StyleColors.textDisabled
        }
    }

    down.indicator: Rectangle {
        x: control.mirrored ? parent.width - width : 0
        height: parent.height
        implicitWidth: height
        implicitHeight: StyleSizes.spinBoxHeight
        color: palette.dark
        border.color: StyleColors.border
        border.width: control.down.hovered ? 1 : 0

        Label {
            text: FontAwesome.minus
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            anchors.centerIn: parent
            color: enabled ? palette.text : StyleColors.textDisabled
        }
    }
    background: Rectangle {
        implicitWidth: 140
        color: "transparent"
        border.color: StyleColors.border
    }
}
