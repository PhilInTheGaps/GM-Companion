import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."

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
                        if (AddonRepositoryManager.addRepository(add_text_field.text)) {
                            add_text_field.clear()
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
                model: AddonRepositoryManager ? AddonRepositoryManager.repositories : []

                Item {
                    id: repo_delegate

                    required property AddonRepository modelData
                    required property int index

                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: text_field.height

                    TextField {
                        id: text_field
                        text: repo_delegate.modelData.url
                        readOnly: true
                        selectByMouse: true
                        anchors.left: parent.left
                        anchors.right: delete_button.left
                    }

                    Button {
                        id: delete_button
                        text: qsTr("Remove")
                        enabled: !repo_delegate.modelData.isDefault

                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom

                        onClicked: {
                            AddonRepositoryManager.removeRepository(repo_delegate.index)
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
                visible: AddonManager.addons.length < 1
            }

            Repeater {
                model: AddonManager.addons

                AddonItem {
                    required property Addon modelData

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
        width: Sizes.sidebarWidth

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

                enabled: !AddonRepositoryManager.isLoading

                onClicked: AddonManager.refresh()
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
