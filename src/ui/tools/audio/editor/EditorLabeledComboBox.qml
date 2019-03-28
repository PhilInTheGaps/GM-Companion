import QtQuick 2.9
import QtQuick.Controls 2.2

Column {
    id: root

    property string text
    property alias currentText: combobox.currentText
    property alias currentIndex: combobox.currentIndex
    signal comboboxTextChanged(string currentText)
    property alias combo_box: combobox

    width: parent.width - parent.padding * 2

    Text {
        text: root.text
        anchors.left: parent.left
        anchors.right: parent.right
        color: color_scheme.textColor
    }

    ComboBox {
        id: combobox
        anchors.left: parent.left
        anchors.right: parent.right

        onCurrentTextChanged: {
            root.comboboxTextChanged(currentText)
        }
    }
}
