import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts
import "../../defines.js" as Defines
import "../../colors.js" as Colors

Item {
    id: root

    Dialog {
        id: repository_dialog

        title: qsTr("Manage Repositories")

        x: flickable.anchors.leftMargin
        y: flickable.anchors.topMargin

        width: flickable.width
        height: flickable.height
        modal: true

        standardButtons: Dialog.Close

        onOpened: {
            add_text_field.focus = true
        }

        Column {
            spacing: 10
            anchors.fill: parent

            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                height: add_text_field.height

                TextField {
                    id: add_text_field
                    placeholderText: qsTr("Enter repository url here")
                    selectByMouse: true
                    anchors.left: parent.left
                    anchors.right: add_button.left
                }

                Button {
                    id: add_button
                    text: qsTr("Add")

                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom

                    onClicked: {
                        if (addon_repository_manager) {
                            if (addon_repository_manager.addRepository(
                                        add_text_field.text)) {
                                add_text_field.clear()
                            }
                        }
                    }
                }
            }

            Rectangle {
                color: Colors.menuSeparator
                height: 1
                anchors.left: parent.left
                anchors.right: parent.right
            }

            Repeater {
                model: addon_repository_manager ? addon_repository_manager.repositories : []

                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: text_field.height

                    TextField {
                        id: text_field
                        text: modelData.url
                        readOnly: true
                        selectByMouse: true
                        anchors.left: parent.left
                        anchors.right: delete_button.left
                    }

                    Button {
                        id: delete_button
                        text: qsTr("Remove")
                        enabled: !modelData.isDefault

                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        onClicked: {
                            if (addon_repository_manager) {
                                addon_repository_manager.removeRepository(index)
                            }
                        }
                    }
                }
            }
        }
    }

    Flickable {
        id: flickable
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: right_item.left
        anchors.margins: 10

        clip: true
        contentHeight: addon_column.implicitHeight
        contentWidth: -1
        interactive: true

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: flickable.contentHeight > flickable.height
        }

        Column {
            id: addon_column
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 10

            Label {
                text: qsTr("No addons available")
                anchors.horizontalCenter: parent.horizontalCenter
                visible: addon_manager.addons.length < 1
            }

            Repeater {
                model: addon_manager.addons

                AddonItem {
                    addon: modelData

                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }
        }
    }

    Rectangle {
        id: right_item
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: Defines.SIDEBAR_WIDTH

        color: palette.dark

        Column {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            CustomButton {
                anchors.left: parent.left
                anchors.right: parent.right

                buttonText: qsTr("Refresh")
                iconText: FontAwesome.rotate

                enabled: !addon_repository_manager.isLoading

                onClicked: addon_manager.refresh()
            }

            CustomButton {
                anchors.left: parent.left
                anchors.right: parent.right

                buttonText: qsTr("Edit Repositories")
                iconText: FontAwesome.pen

                onClicked: repository_dialog.open()
            }
        }
    }
}
