import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import IconFonts

Label {
    id: root

    required property T.AbstractButton control
    required property int checkState

    font.pixelSize: control.height > 26 ? 26 : control.height
    x: root.control.text ? (root.control.mirrored ? root.control.width - width - root.control.rightPadding : root.control.leftPadding) : root.control.leftPadding
                           + (root.control.availableWidth - width) / 2
    y: parent.height / 2 - height / 2
    color: StyleColors.border

    text: FontAwesome.square
    font.family: FontAwesome.fontRegular.family
    font.styleName: FontAwesome.fontRegular.styleName
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    opacity: enabled ? 1 : 0.3

    Label {
        text: root.checkState === Qt.Checked ? FontAwesome.check : FontAwesome.square
        anchors.centerIn: parent
        color: root.control.down ? StyleColors.focus : root.control.enabled ? palette.text : StyleColors.textDisabled
        visible: root.checkState !== Qt.Unchecked
        font.pixelSize: root.font.pixelSize / 2
        font.family: FontAwesome.fontSolid.family
        font.styleName: FontAwesome.fontSolid.styleName

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
