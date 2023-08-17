import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts
import "../../defines.js" as Defines

Page {
    id: root
    signal backToTool

    enum Mode {
        Project = 0,
        Category = 1,
        Unit = 2
    }

    function modeToString(mode) {
        switch (mode) {
        case ConverterEditor.Mode.Project:
            return qsTr("project")
        case ConverterEditor.Mode.Category:
            return qsTr("category")
        case ConverterEditor.Mode.Unit:
            return qsTr("unit")
        }
    }

    ConverterEditorNewThingDialog {
        id: new_thing_dialog
    }

    Dialog {
        id: delete_dialog
        modal: true
        title: qsTr("Delete ") + modeToString(delete_dialog.mode) + " "
               + (delete_dialog.element ? delete_dialog.element.name : "") + "?"

        property int mode: ConverterEditor.Mode.Project
        property var element: undefined

        standardButtons: Dialog.Yes | Dialog.No

        contentItem: Label {
            text: qsTr("This can not be undone!")
        }

        onAccepted: {
            switch (delete_dialog.mode) {
            case ConverterEditor.Mode.Project:
                converter_editor.deleteProject(element)
                break
            case ConverterEditor.Mode.Category:
                converter_editor.deleteCategory(element)
                break
            case ConverterEditor.Mode.Unit:
                converter_editor.deleteUnit(element)
                break
            }
        }

        function center() {
            x = (root.width - width) / 2
            y = 0
        }
    }

    Dialog {
        id: rename_dialog
        property var element: undefined
        property int mode: ConverterEditor.Mode.Project

        modal: true
        title: ""

        contentItem: Column {
            TextField {
                id: rename_field
                text: (rename_dialog.element ? rename_dialog.element.name : "")
                selectByMouse: true
                onAccepted: rename_dialog.accept()
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onOpened: rename_field.forceActiveFocus()

        onAccepted: {
            switch (rename_dialog.mode) {
            case ConverterEditor.Mode.Project:
                converter_editor.renameProject(element, rename_field.text)
                break
            case ConverterEditor.Mode.Category:
                converter_editor.renameCategory(element, rename_field.text)
                break
            case ConverterEditor.Mode.Unit:
                converter_editor.renameUnit(element, rename_field.text)
                break
            default:
                console.log("Error: Rename dialog in undefined mode")
            }
        }

        function center() {
            x = (root.width - width) / 2
            y = 0
        }
    }

    header: CustomToolBar {
        id: tool_bar

        enableBack: true
        enableAdd: true
        enableSave: true
        enableExport: false

        isSaved: converter_editor ? converter_editor.isSaved : true

        onBackClicked: root.backToTool()
        onAddClicked: new_thing_dialog.open()
        onSaveClicked: converter_editor.save()

        Row {
            anchors.left: tool_bar.button_row.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            padding: 10
            spacing: 5

            CustomToolBarComboBox {
                id: project_combo_box

                width: 150
                emptyString: qsTr("No Projects")
                textRole: "name"

                model: converter_editor ? converter_editor.projects : []

                onCurrentIndexChanged: {
                    if (!converter_editor
                            || currentIndex >= converter_editor.projects.length)
                        return

                    converter_editor.currentProject = converter_editor.projects[currentIndex]
                }
            }

            CustomToolBarButton {
                anchors.margins: 10
                iconText: FontAwesome.pen
                toolTipText: qsTr("Rename Project")
                enabled: project_combo_box.model.length > 0
                onClicked: {
                    rename_dialog.mode = ConverterEditor.Mode.Project
                    rename_dialog.title = qsTr("Rename Project")
                    rename_dialog.element = converter_editor.currentProject
                    rename_dialog.x = project_combo_box.x
                    rename_dialog.y = 0
                    rename_dialog.open()
                }
            }

            CustomToolBarButton {
                anchors.margins: 10
                iconText: FontAwesome.trash
                toolTipText: qsTr("Delete Project")
                enabled: project_combo_box.model.length > 0
                onClicked: {
                    delete_dialog.x = project_combo_box.x
                    delete_dialog.y = 0
                    delete_dialog.element = converter_editor.currentProject
                    delete_dialog.mode = ConverterEditor.Mode.Project
                    delete_dialog.open()
                }
            }
        }
    }

    Rectangle {
        id: side_bar
        color: palette.dark
        width: Defines.SIDEBAR_WIDTH

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Flickable {
            id: catetory_view

            anchors.fill: parent
            anchors.margins: 5

            clip: true
            contentWidth: -1
            contentHeight: category_column.implicitHeight
            interactive: catetory_view_scroll_bar.visible

            ScrollBar.vertical: ScrollBar {
                id: catetory_view_scroll_bar
                anchors.right: parent.right
                visible: catetory_view.contentHeight > catetory_view.height
            }

            Column {
                id: category_column
                anchors.left: parent.left
                anchors.right: parent.right

                anchors.rightMargin: catetory_view_scroll_bar.visible ? catetory_view_scroll_bar.width + 5 : 0

                Repeater {
                    id: categories_repeater

                    model: converter_editor
                           && converter_editor.currentProject ? converter_editor.currentProject.categories : []

                    CustomButton {
                        buttonText: modelData.name

                        anchors.left: parent.left
                        anchors.right: parent.right

                        onClicked: {
                            if (!converter_editor)
                                return

                            converter_editor.currentCategory = modelData
                        }

                        Row {
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right

                            CustomButton {
                                iconText: FontAwesome.pen
                                transparentBackground: true

                                anchors.top: parent.top
                                anchors.bottom: parent.bottom

                                onClicked: {
                                    rename_dialog.x = project_combo_box.x
                                    rename_dialog.y = 0
                                    rename_dialog.element = modelData
                                    rename_dialog.mode = ConverterEditor.Mode.Category
                                    rename_dialog.open()
                                }
                            }

                            CustomButton {
                                iconText: FontAwesome.trash
                                transparentBackground: true

                                anchors.top: parent.top
                                anchors.bottom: parent.bottom

                                onClicked: {
                                    delete_dialog.x = project_combo_box.x
                                    delete_dialog.y = 0
                                    delete_dialog.element = modelData
                                    delete_dialog.mode = ConverterEditor.Mode.Category
                                    delete_dialog.open()
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Item {
        id: main_view

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: side_bar.right
        anchors.right: parent.right
        anchors.margins: 5

        Flickable {
            id: item_view

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: Math.min(item_column.implicitWidth + 10, parent.width)

            clip: true
            contentHeight: item_column.implicitHeight
            interactive: item_view_scroll_bar.visible

            ScrollBar.vertical: ScrollBar {
                id: item_view_scroll_bar
                anchors.right: parent.right
                visible: item_view.contentHeight > item_view.height
            }

            Column {
                id: item_column
                spacing: 10
                padding: 10

                Repeater {
                    id: items_repeater

                    model: converter_editor
                           && converter_editor.currentCategory ? converter_editor.currentCategory.units : []

                    Rectangle {
                        color: palette.dark
                        border.color: palette.button
                        border.width: 1

                        width: item_row.width
                        height: Defines.TOOLBAR_HEIGHT

                        Row {
                            id: item_row

                            anchors.top: parent.top
                            anchors.bottom: parent.bottom

                            spacing: 10
                            padding: 10

                            Label {
                                text: qsTr("Name:")

                                anchors.top: parent.top
                                anchors.bottom: parent.bottom

                                verticalAlignment: Label.AlignVCenter
                                horizontalAlignment: Label.AlignHCenter
                            }

                            TextField {
                                text: modelData.name
                                selectByMouse: true
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom

                                onTextEdited: {
                                    converter_editor.renameUnit(modelData, text)
                                }
                            }

                            Label {
                                text: qsTr("Value:")
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                verticalAlignment: Label.AlignVCenter
                                horizontalAlignment: Label.AlignLeft
                            }

                            TextField {
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                text: Number(modelData.value).toLocaleString(
                                          Qt.locale(
                                              settings_manager.languageBcp47),
                                          'G', 8)

                                selectByMouse: true

                                validator: DoubleValidator {
                                    notation: DoubleValidator.ScientificNotation
                                    locale: settings_manager.languageBcp47
                                }

                                onEditingFinished: {
                                    converter_editor.changeUnitValue(modelData,
                                                                     text)
                                }
                            }

                            CustomButton {
                                iconText: FontAwesome.trash
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                transparentBackground: true

                                onClicked: {
                                    delete_dialog.center()
                                    delete_dialog.element = modelData
                                    delete_dialog.mode = ConverterEditor.Mode.Unit
                                    delete_dialog.open()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
