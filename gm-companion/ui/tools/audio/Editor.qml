import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import gm.companion.audioeditor 1.0
import gm.companion.colorscheme 1.0
import gm.companion.settingstool 1.0
import "../../fontawesome"

Page {
    id: audio_editor

    signal backToTool
    signal projectAdded

    AudioEditor {
        id: editor_tool

        Component.onCompleted: updateProjectList()

        onProjectListChanged: project_combo_box.model = getProjectList()
        onCategoryListChanged: category_combo_box.model = getCategoryList()
        onScenarioListChanged: scenario_combo_box.model = getScenarioList()

        onElementListChanged: {
            populateElementColumn()
        }

        onListChanged: {
            audio_list_page.basePath = getCurrentBasePath()

            audio_list_page.list_index = getListIndex()

            audio_list_page.populateTable(getCurrentFileNames(),
                                          getCurrentFilePaths(),
                                          getCurrentFileMissing())
        }

        onUrlChanged: radio_page.setURL(getURL())

        onSpotifyIDChanged: spotify_page.setID(getID())

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

        function moveElementUp(element, type) {
            moveElement(element, type, -1)
        }

        function moveElementDown(element, type) {
            moveElement(element, type, 1)
        }

        function populateElementColumn() {
            elements_column.children = []

            for (var i = 0; i < getMusicLists().length; i++) {
                var component = Qt.createComponent("./EditorElementButton.qml")

                var button = component.createObject(elements_column, {
                                                        "element": getMusicLists(
                                                                       )[i],
                                                        "type": 0
                                                    })
                button.clicked.connect(selectList)
                button.remove.connect(deleteList)
                button.moveUp.connect(moveElementUp)
                button.moveDown.connect(moveElementDown)
            }

            for (var i = 0; i < getSoundLists().length; i++) {
                var component = Qt.createComponent("./EditorElementButton.qml")

                var button = component.createObject(elements_column, {
                                                        "element": getSoundLists(
                                                                       )[i],
                                                        "type": 1
                                                    })
                button.clicked.connect(selectList)
                button.remove.connect(deleteList)
                button.moveUp.connect(moveElementUp)
                button.moveDown.connect(moveElementDown)
            }

            for (var i = 0; i < getRadios().length; i++) {
                var component = Qt.createComponent("./EditorElementButton.qml")

                var button = component.createObject(elements_column, {
                                                        "element": getRadios(
                                                                       )[i],
                                                        "type": 2
                                                    })
                button.clicked.connect(selectRadio)
                button.remove.connect(deleteList)
                button.moveUp.connect(moveElementUp)
                button.moveDown.connect(moveElementDown)
            }

            for (var i = 0; i < getSpotifyPlaylists().length; i++) {
                var component = Qt.createComponent("./EditorElementButton.qml")

                var button = component.createObject(elements_column, {
                                                        "element": getSpotifyPlaylists(
                                                                       )[i],
                                                        "type": 3
                                                    })
                button.clicked.connect(selectSpotifyList)
                button.remove.connect(deleteList)
                button.moveUp.connect(moveElementUp)
                button.moveDown.connect(moveElementDown)
            }
        }
    }

    SettingsTool {
        id: settings_tool
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Dialog {
        id: new_project_dialog
        title: qsTr("Create New Project")

        contentItem: Column {
            TextField {
                id: project_textfield
                placeholderText: qsTr("Project Name")
                selectByMouse: true
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            editor_tool.createProject(project_textfield.text)
            project_textfield.clear()
            projectAdded()
        }
    }

    Dialog {
        id: new_category_dialog
        title: qsTr("Create New Category")

        contentItem: Column {
            TextField {
                id: category_textfield
                placeholderText: qsTr("Category Name")
                selectByMouse: true
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            editor_tool.createCategory(category_textfield.text)
            category_textfield.clear()
        }
    }

    Dialog {
        id: new_scenario_dialog
        title: qsTr("Create New Scenario")

        contentItem: Column {
            TextField {
                id: scenario_textfield
                placeholderText: qsTr("Scenario Name")
                selectByMouse: true
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            editor_tool.createScenario(scenario_textfield.text)
            scenario_textfield.clear()
        }
    }

    Dialog {
        id: new_element_dialog
        title: qsTr("Create New Element")

        contentItem: Column {
            spacing: 5

            Text {
                text: qsTr("Element Type")
            }

            ComboBox {
                id: element_combo_box
                model: [qsTr("Music List"), qsTr("Sound List"), qsTr(
                        "Radio"), qsTr("Spotify Playlist")]
                width: element_textfield.width
            }

            TextField {
                id: element_textfield
                placeholderText: qsTr("Element Name")
                selectByMouse: true
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            if (element_textfield.text != "") {

                editor_tool.createList(element_textfield.text,
                                       element_combo_box.currentIndex)

                element_textfield.clear()
            }
        }
    }

    AudioExporter {
        id: audio_exporter
        width: parent.width - 100
        height: parent.height - 100
        x: 50
        y: 50
    }

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

            ToolButton {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.far_arrow_alt_circle_left
                    pointSize: 25
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                width: height

                onClicked: backToTool()
            }

            ToolButton {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: new_project_text.width + 10
                hoverEnabled: true

                Text {
                    id: new_project_text
                    text: qsTr("New Project")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: new_project_dialog.open()
            }

            ToolButton {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: save_project_text.width + 10
                hoverEnabled: true

                Text {
                    id: save_project_text
                    text: qsTr("Save Project")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: editor_tool.saveProject()
            }

            ToolButton {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: new_category_text.width + 10
                hoverEnabled: true

                Text {
                    id: new_category_text
                    text: qsTr("New Category")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: new_category_dialog.open()
            }

            ToolButton {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: new_scenario_text.width + 10
                hoverEnabled: true

                Text {
                    id: new_scenario_text
                    text: qsTr("New Scenario")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: new_scenario_dialog.open()
            }

            ToolButton {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: new_element_text.width + 10
                hoverEnabled: true

                Text {
                    id: new_element_text
                    text: qsTr("New Element")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: new_element_dialog.open()
            }

            ToolButton {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: export_audio_files.width + 10
                hoverEnabled: true

                Text {
                    id: export_audio_files
                    text: qsTr("Export Audio Files")
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    font.bold: true
                    font.pointSize: 12
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    audio_exporter.project = project_combo_box.currentText
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

            Text {
                id: project_text
                text: qsTr("Project")
                color: color_scheme.textColor
            }

            ComboBox {
                id: project_combo_box
                width: parent.width - parent.padding * 2

                onCurrentTextChanged: {
                    if (currentText != "") {
                        editor_tool.setCurrentProject(currentText)
                    }
                }
            }

            Text {
                id: category_text
                text: qsTr("Category")
                width: parent.width - parent.padding * 2
                color: color_scheme.textColor
            }

            ComboBox {
                id: category_combo_box
                width: parent.width - parent.padding * 2

                onCurrentTextChanged: {
                    editor_tool.setCurrentCategory(currentText)
                }
            }

            Text {
                id: scenario_text
                text: qsTr("Scenario")
                color: color_scheme.textColor
            }

            ComboBox {
                id: scenario_combo_box
                width: parent.width - parent.padding * 2

                onCurrentTextChanged: {
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

                    Icon {
                        icon: icons.fas_sort_alpha_down
                        pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: qsTr("Sort Alphabetically")
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            ScrollView {
                id: elements_scroll_view
                width: parent.width - parent.padding * 2
                height: parent.height - parent.spacing * 8 - parent.topPadding
                        * 2 - project_text.height - project_combo_box.height
                        - category_text.height - category_combo_box.height
                        - scenario_text.height - scenario_combo_box.height
                        - elements_text.height - sort_button.height
                clip: true

                Column {
                    id: elements_column
                    width: parent.parent.width
                    spacing: 5
                }
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

            onAddAllFiles: editor_tool.addFiles(fileNames, filePaths)
        }
    }
}
