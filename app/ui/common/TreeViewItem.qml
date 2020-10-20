import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../defines.js" as Defines

CustomButton {
    id: root

    property string itemIcon: ""

    height: Defines.TOOLBAR_HEIGHT / 2

    anchors.right: parent.right
    anchors.left: parent.left
    anchors.leftMargin: modelData.depth() * 5 + (!modelData.canToggle
                                                 && itemIcon === "" ? 22 : 0)

    iconText: modelData.canToggle ? (modelData.isOpen ? FontAwesome.caretDown : FontAwesome.caretRight) : itemIcon

    textItem.font.pointSize: 10
    buttonText: modelData.name

    onClicked: modelData.toggle()

    onRightClicked: {
        if (contextMenu) {
            contextMenu.open()
        }
    }

    Dialog {
        id: rename_dialog
        x: root.textItem.x
        width: root.width - x
        height: root.height
        padding: 0
        modal: true

        onOpened: {
            rename_field.selectAll()
            rename_field.forceActiveFocus()
        }

        onAccepted: {
            modelData.rename(rename_field.text)
        }

        contentItem: TextField {
            id: rename_field

            text: root.buttonText
            padding: 0

            onAccepted: {
                rename_dialog.accept()
            }
        }
    }

    Dialog {
        id: new_thing_dialog

        property string type: "TYPE"

        width: root.width
        padding: 5
        modal: true

        onOpened: {
            name_field.selectAll()
            name_field.forceActiveFocus()
        }

        onAccepted: {
            modelData.create(new_thing_dialog.type, name_field.text)
        }

        contentItem: TextField {
            id: name_field

            placeholderText: qsTr("Name")

            onAccepted: {
                new_thing_dialog.accept()
            }
        }
    }

    Dialog {
        id: delete_dialog

        title: qsTr("Are you sure?")

        width: root.width
        padding: 5
        modal: true

        onAccepted: {
            modelData.remove()
        }

        contentItem: Item {
            implicitHeight: 40

            Button {
                id: yes_button
                text: qsTr("Yes")

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width / 2 - 5

                onClicked: {
                    delete_dialog.accept()
                }
            }

            Button {
                id: no_button
                text: qsTr("No")

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.left: yes_button.right
                anchors.leftMargin: 10

                onClicked: delete_dialog.reject()
            }
        }
    }

    Menu {
        id: contextMenu
        y: root.height
        modal: true

        CustomMenuItem {
            id: rename_item
            text: qsTr("Rename")

            onTriggered: rename_dialog.open()
        }

        Repeater {
            model: modelData.creatables

            CustomMenuItem {
                id: new_item
                text: qsTr("New") + " " + modelData + " ..."

                onTriggered: {
                    new_thing_dialog.title = text
                    new_thing_dialog.type = modelData
                    new_thing_dialog.y = y
                    new_thing_dialog.open()
                }
            }
        }

        CustomMenuItem {
            id: delete_item
            text: qsTr("Delete")
            textColor: "darkred"

            onTriggered: {
                delete_dialog.open()
            }
        }
    }
}
