import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import IconFonts
import "./colors.js" as Colors

Label {
    id: indicator

    property Item control

    font.pixelSize: control.height > 26 ? 26 : control.height
    x: control.text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding
                      + (control.availableWidth - width) / 2
    y: parent.height / 2 - height / 2
    color: Colors.border

    text: FontAwesome.square
    font.family: FontAwesome.fontRegular.family
    font.styleName: FontAwesome.fontRegular.styleName
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    opacity: enabled ? 1 : 0.3

    Label {
        text: control.checkState === Qt.Checked ? FontAwesome.check : FontAwesome.square
        anchors.centerIn: parent
        color: control.down ? Colors.focus : control.enabled ? Colors.text : Colors.textDisabled
        visible: control.checkState !== Qt.Unchecked
        font.pixelSize: indicator.font.pixelSize / 2
        font.family: FontAwesome.fontSolid.family
        font.styleName: FontAwesome.fontSolid.styleName

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
