import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import FontAwesome 2.0
import "../buttons"
import "../audio_exporter"
import "../../../components"

Page {
    id: editor_root

    signal backToTool
    signal projectAdded

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Connections {
        target: audio_editor

        onCurrentScenarioChanged: {
            if (element_stack_view.currentItem !== no_element_text) {
                element_stack_view.replace(no_element_text)
            }
        }

        onShowInfoBar: {
            info_text.text = message
            info_bar.visible = true
            info_bar_timer.start()
        }
    }

    // New Project, Category, Scenario, Element...
    EditorNewThingDialog {
        id: new_thing_dialog
    }

    AudioExporter {
        id: audio_exporter_dialog
        width: parent.width - 100
        height: parent.height - 100
        x: 50
        y: 50
    }

    Dialog {
        id: large_image_dialog
        modal: true

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        contentItem: Image {
            source: (platform.isWindows ? "file:///" : "file://") + audio_editor.resourcesPath() + element_icon_field.text

            fillMode: Image.PreserveAspectFit
        }
    }

    // Top Bar
    ToolBar {
        id: tool_bar
        width: parent.width
        background: Rectangle {
            color: color_scheme.toolbarColor
        }

        Row {
            width: parent.width
            height: color_scheme.toolbarHeight
            spacing: 5

            // Back
            EditorToolButton {
                labeltext: FontAwesome.arrowAltCircleLeft
                onClicked: backToTool()
            }

            EditorToolButton {
                labeltext: FontAwesome.fileMedical
                onClicked: new_thing_dialog.open()
            }

            EditorToolButton {
                labeltext: FontAwesome.fileDownload
                onClicked: audio_editor.saveProject()

                Text {
                    visible: !audio_editor.isSaved
                    text: FontAwesome.asterisk
                    color: "darkred"
                    font.family: FontAwesome.familySolid
                    anchors.right: parent.right
                    y: parent.height - height * 1.5
                }
            }

            EditorToolButton {
                labeltext: FontAwesome.fileExport
                onClicked: {
                    audio_exporter.project = project_box.currentText
                    audio_exporter_dialog.open()
                }
            }

            ToolBarComboBox {
                id: project_box
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5

                model: audio_editor.projectNames
                currentIndex: audio_editor.projectIndex

                onCurrentIndexChanged: audio_editor.setCurrentProject(
                                           currentIndex)
            }

            ToolBarComboBox {
                id: category_box
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5

                model: audio_editor.categoryNames
                currentIndex: audio_editor.categoryIndex

                onCurrentTextChanged: audio_editor.setCurrentCategory(
                                          currentText)
            }

            ToolBarComboBox {
                id: scenario_box
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5

                model: audio_editor.scenarioNames
                currentIndex: audio_editor.scenarioIndex

                onCurrentTextChanged: {
                    audio_editor.setCurrentScenario(currentText)
                    element_stack_view.pop(null)
                    audio_list_page.visible = false
                    radio_page.visible = false
                    spotify_page.visible = false
                }
            }

            // Sort Elements
            ControlBarButton {
                fa_icon: FontAwesome.sortAlphaDown
                onClicked: audio_editor.sortElements()
            }

            // View Mode
            ControlBarButton {
                fa_icon: element_column.small_mode ? FontAwesome.expand : FontAwesome.compress
                onClicked: element_column.small_mode = !element_column.small_mode
            }
        }
    }

    Row {
        width: parent.width
        height: parent.height - tool_bar.height
        y: tool_bar.height

        Item {
            id: left_item
            width: 180

            anchors.top: parent.top
            anchors.bottom: parent.bottom

            EditorElementColumn {
                id: element_column
            }
        }

        Column {
            id: mid_column
            width: parent.width - left_item.width - 5
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            spacing: 5
            bottomPadding: 5
            topPadding: 5

            Column {
                id: element_info_item
                visible: element_stack_view.currentItem != no_element_text
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 5

                Item {
                    id: element_info_row_0
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: color_scheme.toolbarHeight

                    Item {
                        id: element_up_down
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
                                color: color_scheme.menuColor
                            }

                            Text {
                                text: element_name_field.edit_mode ? FontAwesome.checkCircle : FontAwesome.chevronUp
                                font.family: FontAwesome.familySolid
                                color: element_name_field.edit_mode ? "limegreen" : color_scheme.toolbarTextColor
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            onClicked: {
                                if (element_name_field.edit_mode) {
                                    element_name_field.edit_mode = false
                                    audio_editor.setName(
                                                audio_editor.name,
                                                audio_editor.type,
                                                element_name_field.text)
                                } else {
                                    audio_editor.moveElement(audio_editor.name,
                                                             audio_editor.type,
                                                             -1)
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
                                color: color_scheme.menuColor
                            }

                            Text {
                                text: element_name_field.edit_mode ? FontAwesome.timesCircle : FontAwesome.chevronDown
                                font.family: FontAwesome.familySolid
                                color: element_name_field.edit_mode ? "red" : color_scheme.toolbarTextColor
                                anchors.fill: parent
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            onClicked: {
                                if (element_name_field.edit_mode) {
                                    element_name_field.edit_mode = false
                                    element_name_field.text = audio_editor.name
                                } else {
                                    audio_editor.moveElement(audio_editor.name,
                                                             audio_editor.type,
                                                             1)
                                }
                            }
                        }
                    }

                    TextField {
                        id: element_name_field
                        text: audio_editor.name
                        anchors.left: element_up_down.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        selectByMouse: true

                        property bool edit_mode: false
                        activeFocusOnPress: edit_mode
                        focus: edit_mode

                        Button {
                            anchors.top: parent.top
                            anchors.right: element_delete_button.left
                            anchors.bottom: parent.bottom
                            width: height
                            visible: !element_name_field.edit_mode

                            background: Rectangle {
                                color: "transparent"
                            }

                            Text {
                                text: FontAwesome.pen
                                font.family: FontAwesome.familySolid
                                anchors.fill: parent
                                anchors.margins: 10
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                            }

                            onClicked: element_name_field.edit_mode = true
                        }

                        Button {
                            id: element_delete_button
                            anchors.top: parent.top
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            width: height

                            background: Rectangle {
                                color: color_scheme.menuColor
                            }

                            Text {
                                text: FontAwesome.trashAlt
                                font.family: FontAwesome.familySolid
                                font.pixelSize: height
                                anchors.fill: parent
                                anchors.margins: 10
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                color: "red"
                            }

                            onClicked: element_delete_overlay.visible = true

                            Rectangle {
                                id: element_delete_overlay
                                visible: false
                                anchors.fill: parent
                                color: color_scheme.menuColor

                                Text {
                                    text: FontAwesome.trashAlt
                                    font.family: FontAwesome.familySolid
                                    color: color_scheme.toolbarTextColor
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

                                    Text {
                                        text: FontAwesome.checkCircle
                                        font.family: FontAwesome.familySolid
                                        color: "limegreen"
                                        anchors.fill: parent
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                    }

                                    onClicked: {
                                        audio_editor.deleteElement(
                                                    audio_editor.name,
                                                    audio_editor.type)
                                        element_delete_overlay.visible = false
                                        element_stack_view.replace(
                                                    no_element_text)
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

                                    Text {
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

                Item {
                    id: element_info_row_1
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: color_scheme.toolbarHeight

                    TextField {
                        id: element_icon_field
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: element_icon_image.status
                                       == Image.Ready ? element_icon_image.left : icon_finder.left

                        selectByMouse: true
                        text: audio_editor.icon
                        placeholderText: qsTr("Icon Path (Leave empty for default icon)")

                        onTextChanged: {
                            audio_editor.setIcon(audio_editor.name,
                                                 audio_editor.type, text)
                        }
                    }

                    Image {
                        id: element_icon_image
                        visible: status == Image.Ready
                        source: (platform.isWindows ? "file:///" : "file://") + audio_editor.resourcesPath(
                                    ) + element_icon_field.text
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: icon_finder.left
                        width: height
                    }

                    Text {
                        anchors.fill: element_icon_image
                        anchors.margins: 10
                        text: FontAwesome.expand
                        font.family: FontAwesome.familySolid
                        font.pixelSize: height
                        visible: element_icon_mouse_area.containsMouse
                        color: "white"
                    }

                    MouseArea {
                        id: element_icon_mouse_area
                        anchors.fill: element_icon_image
                        onClicked: large_image_dialog.open()
                        hoverEnabled: true
                    }

                    IconFinder {
                        id: icon_finder
                        anchors.right: parent.right
                        text_field: element_icon_field
                    }
                }
            }

            StackView {
                id: element_stack_view
                width: parent.width
                height: parent.height - parent.topPadding * 2
                        - element_info_item.height - parent.spacing
                clip: true
                topPadding: 5
                bottomPadding: 5

                initialItem: no_element_text

                Column {
                    id: no_element_text
                    Text {
                        text: qsTr("No Element Selected")
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: color_scheme.textColor
                    }
                }

                EditorAudioListPage {
                    id: audio_list_page
                    visible: false
                }

                EditorRadioPage {
                    id: radio_page
                    visible: false
                }

                EditorSpotifyPlaylistPage {
                    id: spotify_page
                    visible: false
                }
            }
        }
    }

    Rectangle {
        id: info_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: color_scheme.toolbarHeight
        color: color_scheme.menuColor
        visible: false

        Timer {
            id: info_bar_timer
            interval: 2000

            onTriggered: {
                info_bar.visible = false
                stop()
            }
        }

        Text {
            id: info_text
            anchors.fill: parent
            anchors.margins: 10
            font.pixelSize: height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: color_scheme.toolbarTextColor
        }
    }
}
