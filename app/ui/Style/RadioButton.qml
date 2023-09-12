import QtQuick
import QtQuick.Templates as T

T.RadioButton {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0, Math.max(
                            contentItem.implicitHeight,
                            indicator ? indicator.implicitHeight : 0) + topPadding + bottomPadding)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: 6
    spacing: 8

    indicator: RadioIndicator {
        x: text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding
                  + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2
        control: control
    }

    contentItem: Label {
        leftPadding: control.indicator
                     && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator
                      && control.mirrored ? control.indicator.width + control.spacing : 0

        text: control.text
        font: control.font
        color: control.down ? StyleColors.textDisabled : palette.text
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        opacity: enabled ? 1 : 0.3
    }
}
