import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import gm.companion.audioeditor 1.0
import gm.companion.colorscheme 1.0
import gm.companion.settingstool 1.0
import FontAwesome 2.0
import "../buttons"
import "../audio_exporter"

Page {
    id: audio_editor

    signal backToTool
    signal projectAdded

    AudioEditor {
        id: editor_tool

        Component.onCompleted: updateProjectList()

        onProjectListChanged: project_box.combo_box.model = getProjectList()
        onCategoryListChanged: category_box.combo_box.model = getCategoryList()
        onScenarioListChanged: scenario_box.combo_box.model = getScenarioList()
        onElementListChanged: element_column.populate()

        // When list of audio files changes
        onListChanged: {
            audio_list_page.basePath = getCurrentBasePath()
            audio_list_page.list_index = getListIndex()
            audio_list_page.populateTable(getCurrentFileNames(),
                                          getCurrentFilePaths(),
                                          getCurrentFileMissing())
        }

        onUrlChanged: radio_page.setURL(getURL()) // Radio URL changes
        onSpotifyIDChanged: spotify_page.setID(getID()) // Spotify ID changes

        // When an audio list (music or sound) is selected
        function selectList(name, type) {
            setCurrentList(name, type)

            audio_list_page.type = type
            audio_list_page.setName(name)
            audio_list_page.setPlaybackMode(getCurrentListMode())
            audio_list_page.resourcesPath = getResourcesPath()
            audio_list_page.setIconPath(getCurrentListIcon())

            file_browser.setType(type)

            if (element_stack_view.currentItem == radio_page
                    || element_stack_view.currentItem == spotify_page)
                element_stack_view.pop(null)

            if (element_stack_view.currentItem != audio_list_page) {
                element_stack_view.push(audio_list_page)

                audio_list_page.visible = true
                radio_page.visible = false
                spotify_page.visible = false
            }
        }

        // When a radio is selected
        function selectRadio(name) {
            setCurrentList(name, 2)
            file_browser.setType(2)

            radio_page.setName(name)
            radio_page.resourcesPath = getResourcesPath()
            radio_page.setLocal(getLocal())
            radio_page.setURL(getURL())
            radio_page.setIcon(getCurrentListIcon())

            if (element_stack_view.currentItem == audio_list_page
                    || element_stack_view.currentItem == spotify_page)
                element_stack_view.pop(null)

            if (element_stack_view.currentItem != radio_page) {
                element_stack_view.push(radio_page)

                audio_list_page.visible = false
                spotify_page.visible = false
                radio_page.visible = true
            }
        }

        // When a spotify playlist is selected
        function selectSpotifyList(name) {
            setCurrentList(name, 3)

            spotify_page.setName(name)
            spotify_page.resourcesPath = getResourcesPath()
            spotify_page.setID(getSpotifyID())
            spotify_page.setIcon(getCurrentListIcon())

            if (element_stack_view.currentItem == audio_list_page
                    || element_stack_view.currentItem == radio_page)
                element_stack_view.pop(null)

            if (element_stack_view.currentItem != spotify_page) {
                element_stack_view.push(spotify_page)

                audio_list_page.visible = false
                radio_page.visible = false
                spotify_page.visible = true
            }
        }
    }

    SettingsTool {
        id: settings_tool
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    // New Project
    EditorNewThingDialog {
        id: new_project_dialog
        title: qsTr("Create New Project")
        placeholderText: qsTr("Project Name")
        onDone: {
            editor_tool.createProject(thing)
            projectAdded()
        }
    }

    // New Category
    EditorNewThingDialog {
        id: new_category_dialog
        title: qsTr("Create New Category")
        placeholderText: qsTr("Category Name")
        onDone: {
            editor_tool.createCategory(thing)
        }
    }

    // New Scenario
    EditorNewThingDialog {
        id: new_scenario_dialog
        title: qsTr("Create New Scenario")
        placeholderText: qsTr("Scenario Name")
        onDone: {
            editor_tool.createScenario(thing)
        }
    }

    // New Element
    EditorNewElementDialog {
        id: new_element_dialog
    }

    AudioExporter {
        id: audio_exporter
        width: parent.width - 100
        height: parent.height - 100
        x: 50
        y: 50
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
                label.font.pixelSize: parent.height - 10
                label.font.family: FontAwesome.familySolid
                label.font.bold: false
                onClicked: backToTool()
            }

            EditorToolButton {
                labeltext: qsTr("New Project")
                onClicked: new_project_dialog.open()
            }

            EditorToolButton {
                labeltext: qsTr("Save Project")
                onClicked: editor_tool.saveProject()
            }

            EditorToolButton {
                labeltext: qsTr("New Category")
                onClicked: new_category_dialog.open()
            }

            EditorToolButton {
                labeltext: qsTr("New Scenario")
                onClicked: new_scenario_dialog.open()
            }

            EditorToolButton {
                labeltext: qsTr("New Element")
                onClicked: new_element_dialog.open()
            }

            EditorToolButton {
                labeltext: qsTr("Export Audio Files")
                onClicked: {
                    audio_exporter.project = project_box.combo_box.currentText
                    audio_exporter.open()
                }
            }
        }
    }

    Row {
        width: parent.width
        height: parent.height - tool_bar.height
        y: tool_bar.height
        spacing: 5

        Column {
            id: left_column
            width: 175
            height: parent.height
            spacing: 5
            padding: 5

            EditorLabeledComboBox {
                id: project_box
                text: qsTr("Project")
                onComboboxTextChanged: {
                    if (currentText != "") {
                        editor_tool.setCurrentProject(currentText)
                    }
                }
            }

            EditorLabeledComboBox {
                id: category_box
                text: qsTr("Category")
                onComboboxTextChanged: editor_tool.setCurrentCategory(
                                           currentText)
            }

            EditorLabeledComboBox {
                id: scenario_box
                text: qsTr("Scenario")
                onComboboxTextChanged: {
                    editor_tool.setCurrentScenario(currentText)
                    element_stack_view.pop(null)
                    audio_list_page.visible = false
                    radio_page.visible = false
                    spotify_page.visible = false
                }
            }

            Text {
                id: elements_text
                text: qsTr("Elements")
                color: color_scheme.textColor
            }

            Button {
                onClicked: editor_tool.sortElements()
                width: parent.width - parent.padding * 2
                id: sort_button

                Row {
                    anchors.centerIn: parent
                    spacing: 5

                    Text {
                        text: FontAwesome.sortAlphaDown
                        font.family: FontAwesome.familySolid
                        font.pixelSize: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: qsTr("Sort Alphabetically")
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            EditorElementColumn {
                id: element_column
            }
        }

        Column {
            id: mid_column
            width: parent.width - parent.spacing * 2 - left_column.width - file_browser.width
            height: parent.height
            spacing: 5
            topPadding: 5
            bottomPadding: 5

            StackView {
                id: element_stack_view
                width: parent.width
                height: parent.height - parent.topPadding * 2
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

                    onRemoveFile: editor_tool.removeFile(fileIndex)
                    onSaveList: editor_tool.saveList(type)
                    onMoveFile: editor_tool.moveFile(index, positions)
                    onChangeMode: editor_tool.setCurrentListMode(mode)
                    onChangeIcon: editor_tool.setCurrentListIcon(path)
                }

                EditorRadioPage {
                    id: radio_page

                    visible: false

                    onSaveRadio: editor_tool.saveList(2)
                    onChangeIcon: editor_tool.setCurrentListIcon(path)
                    onChangeLocal: editor_tool.setLocal(local)
                    onChangeURL: editor_tool.setURL(url)
                }

                EditorSpotifyPlaylistPage {
                    id: spotify_page

                    visible: false

                    onSavePlaylist: editor_tool.saveList(3)
                    onChangeIcon: editor_tool.setCurrentListIcon(path)
                    onChangeID: editor_tool.setSpotifyID(id)
                }
            }
        }

        EditorFileBrowser {
            id: file_browser
            width: 175
            height: parent.height

            visible: element_stack_view.currentItem == no_element_text ? false : true

            onAddFile: {
                if (fileType == 2) {
                    editor_tool.setURL(path)
                } else {
                    editor_tool.addFile(file, path)
                }
            }

            onAddAllFiles: editor_tool.addFiles(fileNames, filePaths, [""])
        }
    }
}
