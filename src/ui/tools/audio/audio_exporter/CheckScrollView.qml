import QtQuick 2.9
import QtQuick.Controls 2.2

Column {
    id: root
    property string headline
    property var repModel
    property var checkedFunc

    signal itemChecked(int index, bool checked)
    signal itemClicked(int index, bool checked)

    spacing: 10
    padding: 10

    Text {
        id: text
        text: headline
        font.bold: true
        color: color_scheme.textColor
    }

    ScrollView {
        id: scroll
        width: parent.width - parent.padding * 2
        height: parent.height - text.height - parent.padding * 2
        clip: true
        contentHeight: col.implicitHeight

        Column {
            id: col

            width: root.width - root.padding * 2
            spacing: 5

            Repeater {
                model: repModel

                Button {
                    id: button
                    width: col.width

                    Row {
                        spacing: 5

                        CheckBox {
                            id: button_checkbox
                            checked: checkedFunc(index)

                            onCheckedChanged: itemChecked(index, checked)
                        }

                        Text {
                            text: modelData
                            anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideRight
                            width: button.width - x
                        }

                        anchors.verticalCenter: parent.verticalCenter
                    }

                    onClicked: {
                        itemClicked(index, button_checkbox.checked)
                    }
                }
            }
        }
    }
}
