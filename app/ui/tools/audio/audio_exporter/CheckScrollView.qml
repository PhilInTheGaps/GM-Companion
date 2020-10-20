import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Item {
    id: root
    property string headline
    property var repModel
    property var checkedFunc

    signal itemChecked(int index, bool checked)
    signal itemClicked(int index, bool checked)

    Label {
        id: text
        text: headline
        font.bold: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: 5
        anchors.rightMargin: 5

        elide: Text.ElideRight
    }

    ListView {
        id: list

        anchors.top: text.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        clip: true
        model: repModel

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
        }

        delegate: CustomButton {
            id: button

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0
            height: 40

            CheckBox {
                id: button_checkbox

                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                checked: checkedFunc(index)

                onCheckedChanged: itemChecked(index, checked)
            }

            leftPadding: button_checkbox.width
            buttonText: modelData

            onClicked: {
                itemClicked(index, button_checkbox.checked)
            }
        }
    }
}
