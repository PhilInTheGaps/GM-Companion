pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import common
import src
import "../.."

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
        case ConverterEditorPage.Mode.Project:
            return qsTr("project")
        case ConverterEditorPage.Mode.Category:
            return qsTr("category")
        case ConverterEditorPage.Mode.Unit:
            return qsTr("unit")
        }
    }

    ConverterEditorNewThingDialog {
        id: new_thing_dialog
    }

    Dialog {
        id: delete_dialog
        modal: true
        title: qsTr("Delete ") + root.modeToString(delete_dialog.mode) + " "
               + (delete_dialog.element ? delete_dialog.element.name : "") + "?"

        property int mode: ConverterEditorPage.Mode.Project
        property var element: undefined

        standardButtons: Dialog.Yes | Dialog.No

        contentItem: Label {
            text: qsTr("This can not be undone!")
        }

        onAccepted: {
            switch (delete_dialog.mode) {
            case ConverterEditorPage.Mode.Project:
                ConverterTool.editor.deleteProject(element)
                break
            case ConverterEditorPage.Mode.Category:
                ConverterTool.editor.deleteCategory(element)
                break
            case ConverterEditorPage.Mode.Unit:
                ConverterTool.editor.deleteUnit(element)
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
        property int mode: ConverterEditorPage.Mode.Project

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
            case ConverterEditorPage.Mode.Project:
                ConverterTool.editor.renameProject(element, rename_field.text)
                break
            case ConverterEditorPage.Mode.Category:
                ConverterTool.editor.renameCategory(element, rename_field.text)
                break
            case ConverterEditorPage.Mode.Unit:
                ConverterTool.editor.renameUnit(element, rename_field.text)
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

        isSaved: ConverterTool.editor ? ConverterTool.editor.isSaved : true

        onBackClicked: root.backToTool()
        onAddClicked: new_thing_dialog.open()
        onSaveClicked: ConverterTool.editor.save()

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

                model: ConverterTool.editor ? ConverterTool.editor.projects : []

                onCurrentIndexChanged: {
                    if (!ConverterTool.editor
                            || currentIndex >= ConverterTool.editor.projects.length)
                        return

                    ConverterTool.editor.currentProject = ConverterTool.editor.projects[currentIndex]
                }
            }

            CustomToolBarButton {
                anchors.margins: 10
                iconText: FontAwesome.pen
                toolTipText: qsTr("Rename Project")
                enabled: project_combo_box.model.length > 0
                onClicked: {
                    rename_dialog.mode = ConverterEditorPage.Mode.Project
                    rename_dialog.title = qsTr("Rename Project")
                    rename_dialog.element = ConverterTool.editor.currentProject
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
                    delete_dialog.element = ConverterTool.editor.currentProject
                    delete_dialog.mode = ConverterEditorPage.Mode.Project
                    delete_dialog.open()
                }
            }
        }
    }

    Rectangle {
        id: side_bar
        color: palette.dark
        width: Sizes.sidebarWidth

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

                    model: ConverterTool.editor.currentProject ? ConverterTool.editor.currentProject.categories : []

                    CustomButton {
                        id: category_delegate
                        required property ConverterCategory modelData

                        buttonText: modelData.name

                        anchors.left: parent ? parent.left : undefined
                        anchors.right: parent ? parent.right : undefined

                        onClicked: {
                            if (!ConverterTool.editor)
                                return

                            ConverterTool.editor.currentCategory = modelData
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
                                    rename_dialog.element = category_delegate.modelData
                                    rename_dialog.mode = ConverterEditorPage.Mode.Category
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
                                    delete_dialog.element = category_delegate.modelData
                                    delete_dialog.mode = ConverterEditorPage.Mode.Category
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

                    model: ConverterTool.editor
                           && ConverterTool.editor.currentCategory ? ConverterTool.editor.currentCategory.units : []

                    Rectangle {
                        id: unit_delegate

                        required property ConverterUnit modelData

                        color: palette.dark
                        border.color: palette.button
                        border.width: 1

                        width: item_row.width
                        height: Sizes.toolbarHeight

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
                                text: unit_delegate.modelData.name
                                selectByMouse: true
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom

                                onTextEdited: {
                                    ConverterTool.editor.renameUnit(unit_delegate.modelData, text)
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
                                text: Number(unit_delegate.modelData.value).toLocaleString(
                                          Qt.locale(
                                              SettingsManager.languageBcp47),
                                          'G', 8)

                                selectByMouse: true

                                validator: DoubleValidator {
                                    notation: DoubleValidator.ScientificNotation
                                    locale: SettingsManager.languageBcp47
                                }

                                onEditingFinished: {
                                    ConverterTool.editor.changeUnitValue(unit_delegate.modelData, text)
                                }
                            }

                            CustomButton {
                                iconText: FontAwesome.trash
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                transparentBackground: true

                                onClicked: {
                                    delete_dialog.center()
                                    delete_dialog.element = unit_delegate.modelData
                                    delete_dialog.mode = ConverterEditorPage.Mode.Unit
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
