import QtQuick
import QtQuick.Controls

ComboBox {
    id: control

    property string emptyString: ""

    Label {
        text: control.emptyString
        anchors.fill: parent
        visible: !control.model || control.model.length < 1
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: control.font.pixelSize
        opacity: 0.7
    }
}
