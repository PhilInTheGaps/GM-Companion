import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.impl 2.2
import FontAwesome 2.0
import "../colors.js" as Colors

Label {
    id: indicator

    property Item control

    font.pixelSize: 26
    x: control.text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding
                      + (control.availableWidth - width) / 2
    y: parent.height / 2 - height / 2
    color: Colors.border

    text: FontAwesome.circle
    font.family: FontAwesome.familyRegular
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    opacity: enabled ? 1 : 0.3

    Label {
        text: FontAwesome.check
        anchors.centerIn: parent
        color: control.down ? Colors.focus : control.enabled ? Colors.text : Colors.textDisabled
        visible: control.checked
        font.pixelSize: 14
        font.family: FontAwesome.familySolid

        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
