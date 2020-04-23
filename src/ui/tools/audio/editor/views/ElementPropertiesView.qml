import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import ".."
import "../../../../defines.js" as Defines

Rectangle {
    property string element_name: ""

    property bool enable_move_buttons: true
    signal moveUp
    signal moveDown

    property bool enable_delete: true
    signal deleteClicked

    anchors.left: parent.left
    anchors.right: parent.right
    height: column.height
    color: "transparent"

    Connections {
        target: unsplash_dialog
        onAccepted: element_icon_field.text = unsplash_dialog.imageSource
    }

    Connections {
        target: audio_editor

        onCurrentElementChanged: {

            subscenario_combo_box_rect.visible = true
            subscenario_combo_box.isSetEnabled = false

            element_name_field.text = audio_editor.name
            element_icon_field.text = audio_editor.icon

            subscenario_combo_box.currentIndex = audio_editor.subscenario + 1
            subscenario_combo_box.isSetEnabled = true
        }
    }

    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        spacing: 2

        // Name, move, delete
        Item {
            id: row0
            anchors.left: parent.left
            anchors.right: parent.right
            height: Defines.TOOLBAR_HEIGHT

            Item {
                id: element_up_down
                visible: enable_move_buttons
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: height / 2

                Button {
                    id: element_up
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: parent.height / 2

                    background: Rectangle {
                        color: palette.alternateBase
                    }

                    Label {
                        text: element_name_field.edit_mode ? FontAwesome.checkCircle : FontAwesome.chevronUp
                        font.family: FontAwesome.familySolid
                        color: element_name_field.edit_mode ? "limegreen" : palette.buttonText
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        if (element_name_field.edit_mode) {
                            element_name_field.edit_mode = false
                            audio_editor.setName(audio_editor.name,
                                                 audio_editor.type,
                                                 element_name_field.text)
                            element_up_down.visible = enable_move_buttons
                        } else {
                            audio_editor.moveElement(audio_editor.name,
                                                     audio_editor.type, -1)
                        }
                    }
                }

                Button {
                    id: element_down
                    anchors.top: element_up.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    background: Rectangle {
                        color: palette.alternateBase
                    }

                    Label {
                        text: element_name_field.edit_mode ? FontAwesome.timesCircle : FontAwesome.chevronDown
                        font.family: FontAwesome.familySolid
                        color: element_name_field.edit_mode ? "red" : palette.buttonText
                        anchors.fill: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        if (element_name_field.edit_mode) {
                            element_name_field.edit_mode = false
                            element_name_field.text = audio_editor.name
                            element_up_down.visible = enable_move_buttons
                        } else {
                            audio_editor.moveElement(audio_editor.name,
                                                     audio_editor.type, 1)
                        }
                    }
                }
            }

            CustomTextField {
                id: element_name_field
                text: element_name
                anchors.left: element_up_down.visible ? element_up_down.right : parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                selectByMouse: true

                property bool edit_mode: false
                activeFocusOnPress: edit_mode
                focus: edit_mode

                rightPadding: leftPadding + (element_delete_button.visible ? element_delete_button.width : 0) + (element_edit_name_button.visible ? element_edit_name_button.width : 0)

                Button {
                    id: element_edit_name_button
                    anchors.top: parent.top
                    anchors.right: enable_delete ? element_delete_button.left : parent.right
                    anchors.bottom: parent.bottom
                    width: enable_delete ? height / 2 : height
                    visible: !element_name_field.edit_mode

                    background: Rectangle {
                        color: "transparent"
                    }

                    Label {
                        text: FontAwesome.pen
                        font.family: FontAwesome.familySolid
                        font.pointSize: 12
                        anchors.fill: parent
                        anchors.margins: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: {
                        element_up_down.visible = true
                        element_name_field.edit_mode = true
                    }
                }

                Button {
                    id: element_delete_button
                    visible: !element_name_field.edit_mode && enable_delete
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    width: height

                    background: Rectangle {
                        color: "transparent"
                    }

                    Label {
                        text: FontAwesome.trashAlt
                        font.family: FontAwesome.familySolid
                        font.pointSize: 12
                        anchors.fill: parent
                        anchors.margins: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }

                    onClicked: element_delete_overlay.visible = true

                    Rectangle {
                        id: element_delete_overlay
                        visible: false
                        anchors.fill: parent
                        color: palette.alternateBase

                        Label {
                            text: FontAwesome.trashAlt
                            font.family: FontAwesome.familySolid
                            font.pixelSize: parent.height / 3
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            width: height / 2
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Button {
                            anchors.top: parent.top
                            anchors.right: parent.right
                            width: parent.width / 2
                            height: parent.height / 2

                            background: Rectangle {
                                color: "transparent"
                            }

                            Label {
                                text: FontAwesome.checkCircle
                                font.family: FontAwesome.familySolid
                                color: "limegreen"
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            onClicked: {
                                deleteClicked()
                                element_delete_overlay.visible = false
                                audio_editor.deleteElement(audio_editor.name,
                                                           audio_editor.type)
                            }
                        }

                        Button {
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            width: parent.width / 2
                            height: parent.height / 2

                            background: Rectangle {
                                color: "transparent"
                            }

                            Label {
                                text: FontAwesome.timesCircle
                                font.family: FontAwesome.familySolid
                                color: "red"
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            onClicked: {
                                element_delete_overlay.visible = false
                            }
                        }
                    }
                }
            }
        }

        // Icon
        Item {
            id: row1
            anchors.left: parent.left
            anchors.right: parent.right
            height: Defines.TOOLBAR_HEIGHT

            CustomTextField {
                id: element_icon_field
                property bool save: false
                anchors.fill: parent

                selectByMouse: true
                text: audio_editor.icon
                placeholderText: qsTr(
                                     "Icon Path (Leave empty for default icon)")

                onTextChanged: {
                    if (save)
                        audio_editor.setIcon(audio_editor.name,
                                             audio_editor.type, text)
                }

                Connections {
                    target: audio_editor
                    onCurrentElementChanged: {
                        element_icon_field.save = false
                        element_icon_field.text = audio_editor.icon
                        element_icon_field.save = true
                    }
                }
            }
        }

        Item {
            id: row2
            anchors.left: parent.left
            anchors.right: parent.right
            height: Defines.TOOLBAR_HEIGHT

            Rectangle {
                color: palette.alternateBase
                anchors.fill: parent
            }

            Rectangle {
                id: subscenario_combo_box_rect
                color: palette.alternateBase
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: image_preview.visible ? image_preview.left : unsplash_finder.left

                CustomToolBarComboBox {
                    id: subscenario_combo_box
                    property bool isSetEnabled: false

                    model: audio_editor.subscenarioNames

                    onCurrentIndexChanged: {
                        if (isSetEnabled) {
                            audio_editor.setSubscenario(audio_editor.name,
                                                        audio_editor.type,
                                                        currentIndex)
                        }
                    }

                    anchors.fill: parent
                }
            }

            Rectangle {
                id: image_preview
                visible: element_icon_image.status == Image.Ready
                color: palette.alternateBase
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: unsplash_finder.left
                width: height

                Image {
                    id: element_icon_image
                    anchors.fill: parent
                    source: if (element_icon_field.text.startsWith("http")) {
                                element_icon_field.text
                            } else {
                                (platform.isWindows ? "file:///" : "file://")
                                        + audio_editor.resourcesPath(
                                            ) + element_icon_field.text
                            }
                }

                Text {
                    anchors.fill: parent
                    anchors.margins: 10
                    text: FontAwesome.expand
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    visible: element_icon_mouse_area.containsMouse
                    color: "white"
                }

                MouseArea {
                    id: element_icon_mouse_area
                    anchors.fill: parent
                    onClicked: large_image_dialog.open()
                    hoverEnabled: true
                }
            }

            Button {
                id: unsplash_finder

                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: icon_finder.left
                width: height

                background: Rectangle {
                    color: palette.alternateBase
                }

                Label {
                    text: FontAwesome.search
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                onClicked: {
                    unsplash_dialog.open()
                }
            }

            IconFinder {
                id: icon_finder
                anchors.right: parent.right
                text_field: element_icon_field
            }
        }
    }
}
