pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import common
import ".."

CustomButton {
    id: root

    property string itemIcon: ""
    required property TreeItem modelData

    height: Sizes.toolbarHeight / 2

    anchors.right: parent ? parent.right : undefined
    anchors.left: parent ? parent.left : undefined
    anchors.leftMargin: modelData ? modelData.depth * 5 + (!modelData.canToggle
                                               && itemIcon === "" ? 22 : 0) : 0

    iconText: modelData && modelData.canToggle ? (modelData.isOpen ? FontAwesome.caretDown : FontAwesome.caretRight) : itemIcon

    textItem.font.pointSize: 10
    buttonText: modelData ? modelData.name : ""
    mainRow.spacing: checkbox.visible ? checkbox.width + 10 : 10

    onClicked: {
        modelData.toggle()
    }

    onRightClicked: {
        if (contextMenu) {
            contextMenu.open()
        }
    }

    CheckBox {
        id: checkbox
        visible: root.modelData && root.modelData.isCheckable
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 2
        width: height
        x: root.iconItem.width + 10

        tristate: true
        checkState: {
            switch (root.modelData ? root.modelData.isChecked : 0) {
            case 0:
                Qt.Unchecked
                break
            case 1:
                Qt.PartiallyChecked
                break
            case 2:
                Qt.Checked
                break
            }
        }

        padding: 0
        spacing: 0

        MouseArea {
            anchors.fill: parent

            onClicked: {
                root.modelData.isChecked = root.modelData.isChecked < 2 ? 2 : 0
            }
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
            root.modelData.rename(rename_field.text)
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
            root.modelData.create(new_thing_dialog.type, name_field.text)
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
            root.modelData.remove()
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
            model: root.modelData ? root.modelData.creatables : []

            CustomMenuItem {
                id: new_item

                required property string modelData

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
