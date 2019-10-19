import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import FontAwesome 2.0
import "../buttons"
import "../audio_exporter"
import "../../../components"
import "views"

Page {
    id: editor_root

    signal backToTool
    signal projectAdded

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Connections {
        target: audio_editor

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

    IconFinderUnsplash {
        id: unsplash_dialog
        x: (parent.width - width - stack.x) / 2
        y: (parent.height - height) / 2
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
            source: if (audio_editor.icon.startsWith("http")) {
                        audio_editor.icon
                    } else {
                        (platform.isWindows ? "file:///" : "file://") + audio_editor.resourcesPath(
                                    ) + audio_editor.icon
                    }

            fillMode: Image.PreserveAspectFit
        }

        onOpened: console.log(contentItem.source)
    }

    EditorRenameDialog {
        id: rename_dialog
    }

    // Top Bar
    CustomToolBar {
        id: tool_bar
        anchors.left: parent.left
        anchors.right: parent.right

        enable_back: true
        enable_add: true
        enable_save: true
        enable_export: true

        is_saved: audio_editor.isSaved

        onBackClicked: backToTool()
        onAddClicked: new_thing_dialog.open()
        onSaveClicked: audio_editor.saveProject()
        onExportClicked: {
            audio_exporter.project = project_box.currentText
            audio_exporter_dialog.open()
        }

        Row {
            anchors.left: tool_bar.button_row.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            padding: 10
            spacing: 10

            ToolBarComboBox {
                id: project_box
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                width: 150

                model: audio_editor.projectNames
                currentIndex: audio_editor.projectIndex

                onCurrentIndexChanged: audio_editor.setCurrentProject(
                                           currentIndex)
            }

            EditorToolButton {
                anchors.margins: 5
                labeltext: FontAwesome.pen
                onClicked: {
                    rename_dialog.mode = 0
                    rename_dialog.title = qsTr("Rename Project")
                    rename_dialog.origName = project_box.currentText
                    rename_dialog.x = project_box.x
                    rename_dialog.y = tool_bar.height
                    rename_dialog.open()
                }
            }

            ToolBarComboBox {
                id: category_box
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                width: 150

                model: audio_editor.categoryNames
                currentIndex: audio_editor.categoryIndex

                onCurrentTextChanged: audio_editor.setCurrentCategory(
                                          currentText)
            }

            EditorToolButton {
                anchors.margins: 5
                labeltext: FontAwesome.pen
                onClicked: {
                    rename_dialog.mode = 1
                    rename_dialog.title = qsTr("Rename Category")
                    rename_dialog.origName = category_box.currentText
                    rename_dialog.x = category_box.x
                    rename_dialog.y = tool_bar.height
                    rename_dialog.open()
                }
            }

            ToolBarComboBox {
                id: scenario_box
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                width: 150

                model: audio_editor.scenarioNames
                currentIndex: audio_editor.scenarioIndex

                onCurrentTextChanged: {
                    audio_editor.setCurrentScenario(currentText)
                }
            }

            EditorToolButton {
                anchors.margins: 5
                labeltext: FontAwesome.pen
                onClicked: {
                    rename_dialog.mode = 2
                    rename_dialog.title = qsTr("Rename Scenario")
                    rename_dialog.origName = scenario_box.currentText
                    rename_dialog.x = scenario_box.x
                    rename_dialog.y = tool_bar.height
                    rename_dialog.open()
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

            ToolBarIconButton {
                fa_icon: FontAwesome.fileAudio
                icon_color: "darkred"

                ToolTip.text: qsTr("Remove missing files.")
                ToolTip.visible: hovered

                Label {
                    text: FontAwesome.trashAlt
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height

                    height: parent.height / 3
                    width: height
                    x: parent.width - width * 1.5
                    y: parent.height - height * 1.5

                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"

                    background: Rectangle {
                        color: color_scheme.menuColor
                    }
                }

                onClicked: audio_editor.removeMissingFiles(audio_editor.name,
                                                           audio_editor.type)
            }
        }
    }

    Item {
        id: main_item
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: tool_bar.bottom
        anchors.bottom: parent.bottom

        Item {
            id: left_item
            width: 220

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            EditorElementColumn {
                id: element_column
            }
        }

        EditorFileListView {
            id: mid_item

            anchors.left: left_item.right
            anchors.right: right_item.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            anchors.topMargin: 5
            anchors.bottomMargin: 5
        }

        Item {
            id: right_item
            width: 250

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.bottomMargin: 5

            EditorRightColumn {
                id: right_column
                anchors.fill: parent
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
