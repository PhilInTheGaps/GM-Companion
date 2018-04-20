import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as Controls1_4

import gm.companion.audioeditor 1.0

Page {
    id: audio_editor

    signal backToTool

    AudioEditor {
        id: editor_tool

        Component.onCompleted: updateProjectList()

        onProjectListChanged: project_combo_box.model = getProjectList()
        onCategoryListChanged: category_combo_box.model = getCategoryList()
        onScenarioListChanged: scenario_combo_box.model = getScenarioList()

        onElementListChanged: {
            populateElementColumn()
        }

        onListChanged: audio_list_page.populateTable(getCurrentFileNames())
        onUrlChanged: radio_page.setURL(getURL())

        function selectList(name, type) {
            setCurrentList(name, type)

            audio_list_page.type = type
            audio_list_page.setName(name)
            audio_list_page.setPlaybackMode(getCurrentListMode())
            audio_list_page.resourcesPath = getResourcesPath()
            audio_list_page.setIconPath(getCurrentListIcon())

            file_browser.setType(type)

            element_swipe_view.setCurrentIndex(1)
        }

        function selectRadio(name) {
            setCurrentList(name, 2)
            file_browser.setType(2)

            radio_page.setName(name)
            radio_page.resourcesPath = getResourcesPath()
            radio_page.setLocal(getLocal())
            radio_page.setURL(getURL())
            radio_page.setIcon(getCurrentListIcon())

            element_swipe_view.setCurrentIndex(2)
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
                                                        x: 0,
                                                        y: 0,
                                                        element: getMusicLists(
                                                                     )[i],
                                                        type: 0
                                                    })
                button.clicked.connect(selectList)
                button.remove.connect(deleteList)
                button.moveUp.connect(moveElementUp)
                button.moveDown.connect(moveElementDown)
            }

            for (var i = 0; i < getSoundLists().length; i++) {
                var component = Qt.createComponent("./EditorElementButton.qml")

                var button = component.createObject(elements_column, {
                                                        x: 0,
                                                        y: 0,
                                                        element: getSoundLists(
                                                                     )[i],
                                                        type: 1
                                                    })
                button.clicked.connect(selectList)
                button.remove.connect(deleteList)
                button.moveUp.connect(moveElementUp)
                button.moveDown.connect(moveElementDown)
            }

            for (var i = 0; i < getRadios().length; i++) {
                var component = Qt.createComponent("./EditorElementButton.qml")

                var button = component.createObject(elements_column, {
                                                        x: 0,
                                                        y: 0,
                                                        element: getRadios()[i],
                                                        type: 2
                                                    })
                button.clicked.connect(selectRadio)
                button.remove.connect(deleteList)
                button.moveUp.connect(moveElementUp)
                button.moveDown.connect(moveElementDown)
            }
        }
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
                model: [qsTr("Music List"), qsTr("Sound List"), qsTr("Radio")]
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

    ToolBar {
        id: tool_bar
        width: parent.width

        Row {
            width: parent.width
            spacing: 5

            ToolButton {
                Image {
                    source: "/icons/media/playBackwards.png"
                    width: parent.height * 0.9
                    height: width

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    sourceSize.width: width
                    sourceSize.height: height
                }

                onClicked: backToTool()
            }

            ToolButton {
                text: qsTr("New Project")
                onClicked: new_project_dialog.open()
            }

            ToolButton {
                text: qsTr("Save Project")
                onClicked: editor_tool.saveProject()
            }

            ToolButton {
                text: qsTr("New Category")
                onClicked: new_category_dialog.open()
            }

            ToolButton {
                text: qsTr("New Scenario")
                onClicked: new_scenario_dialog.open()
            }

            ToolButton {
                text: qsTr("New Element")
                onClicked: new_element_dialog.open()
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
            width: parent.width / 6
            height: parent.height
            spacing: 5
            padding: 5

            Text {
                id: project_text
                text: qsTr("Project")
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
            }

            ComboBox {
                id: scenario_combo_box
                width: parent.width - parent.padding * 2

                onCurrentTextChanged: {
                    editor_tool.setCurrentScenario(currentText)
                    element_swipe_view.setCurrentIndex(0)
                }
            }

            Text {
                id: elements_text
                text: qsTr("Elements")
            }

            ScrollView {
                id: elements_scroll_view
                width: parent.width - parent.padding * 2
                height: parent.height - parent.spacing * 7 - parent.topPadding
                        * 2 - project_text.height - project_combo_box.height
                        - category_text.height - category_combo_box.height - scenario_text.height
                        - scenario_combo_box.height - elements_text.height
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

            SwipeView {
                id: element_swipe_view
                width: parent.width
                height: parent.height
                clip: true
                topPadding: 5
                bottomPadding: 5
                interactive: false

                Column {
                    Text {
                        text: qsTr("No Element Selected")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }

                EditorAudioListPage {
                    id: audio_list_page

                    onRemoveFile: editor_tool.removeFile(fileIndex)
                    onSaveList: editor_tool.saveList(type)
                    onMoveFile: editor_tool.moveFile(index, positions)
                    onChangeMode: editor_tool.setCurrentListMode(mode)
                    onChangeIcon: editor_tool.setCurrentListIcon(path)
                }

                EditorRadioPage {
                    id: radio_page

                    onSaveRadio: editor_tool.saveList(2)
                    onChangeIcon: editor_tool.setCurrentListIcon(path)
                    onChangeLocal: editor_tool.setLocal(local)
                    onChangeURL: editor_tool.setURL(url)
                }
            }
        }

        EditorFileBrowser {
            id: file_browser
            width: parent.width / 6
            height: parent.height

            visible: element_swipe_view.currentIndex == 0 ? false : true

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
