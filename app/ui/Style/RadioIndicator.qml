import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import IconFonts

Label {
    id: indicator

    property T.RadioButton control

    font.pixelSize: 26
    x: control.text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
    y: parent.height / 2 - height / 2
    color: control.down ? StyleColors.buttonPressed : palette.button

    text: FontAwesome.circle
    font.family: FontAwesome.fontRegular.family
    font.styleName: FontAwesome.fontRegular.styleName
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    opacity: enabled ? 1 : 0.3

    Label {
        text: FontAwesome.check
        anchors.centerIn: parent
        color: indicator.control.down ? StyleColors.focus : indicator.control.enabled ? palette.text : StyleColors.textDisabled
        visible: indicator.control.checked
        font.pixelSize: 14
        font.family: FontAwesome.fontSolid.family
        font.styleName: FontAwesome.fontSolid.styleName

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
