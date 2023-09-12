import QtQuick
import QtQuick.Controls
import IconFonts
import src
import common

Dialog {
    id: root
    title: qsTr("New")
    modal: true

    enum Mode {
        Project = 0,
        Category = 1,
        Unit = 2
    }

    property bool canAccept: true
    property alias currentMode: type_combo_box.currentIndex
    property int lastMode: ConverterEditorNewThingDialog.Mode.Project

    function updateCanAccept() {
        if (!ConverterTool.editor) {
            canAccept = false
            return
        }

        switch (currentMode) {
        case ConverterEditorNewThingDialog.Mode.Project:
            canAccept = true
            break
        case ConverterEditorNewThingDialog.Mode.Category:
            canAccept = ConverterTool.editor.currentProject
            error_label.text = qsTr("No Project!")
            break
        case ConverterEditorNewThingDialog.Mode.Unit:
            canAccept = ConverterTool.editor.currentCategory
            error_label.text = qsTr("No Category!")
            break
        default:
            canAccept = false
            error_label.text = qsTr("Unknown Error")
            break
        }
    }

    contentItem: Column {
        spacing: 10

        ComboBox {
            id: type_combo_box
            model: [qsTr("Project"), qsTr("Category"), qsTr("Unit")]
            width: name_field.width

            onCurrentIndexChanged: {
                root.updateCanAccept()
                name_field.forceActiveFocus()
            }
        }

        TextField {
            id: name_field
            placeholderText: qsTr("Element Name")
            selectByMouse: true
            enabled: root.canAccept
            visible: root.canAccept
            onAccepted: {
                if (root.currentMode !== ConverterEditorNewThingDialog.Mode.Unit) {
                    root.accept()
                    return
                }

                if (value_field.acceptableInput) {
                    root.accept()
                }
            }
        }

        TextField {
            id: value_field
            text: qsTr("1.0")
            placeholderText: qsTr("Unit Value")
            selectByMouse: true
            onAccepted: root.accept()
            enabled: root.canAccept
            visible: root.canAccept
                     && root.currentMode === ConverterEditorNewThingDialog.Mode.Unit

            validator: DoubleValidator {
                notation: DoubleValidator.ScientificNotation
                locale: SettingsManager.languageBcp47
            }
        }

        Label {
            id: error_label
            visible: !root.canAccept

            text: qsTr("Unknown Error")
            leftPadding: error_icon.width + 10
            font.pointSize: 12

            Label {
                id: error_icon
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 5

                text: FontAwesome.triangleExclamation
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                font.pointSize: 12
                color: "red"
            }
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onOpened: {
        type_combo_box.currentIndex = lastMode

        name_field.forceActiveFocus()
    }

    onAccepted: {
        if (name_field.text !== "" && canAccept) {

            switch (currentMode) {
            case ConverterEditorNewThingDialog.Mode.Project:
                ConverterTool.editor.createProject(name_field.text)
                break
            case ConverterEditorNewThingDialog.Mode.Category:
                ConverterTool.editor.createCategory(name_field.text)
                break
            case ConverterEditorNewThingDialog.Mode.Unit:
                ConverterTool.editor.createUnit(name_field.text, value_field.text)
                break
            }
        }

        name_field.clear()
    }

    onClosed: {
        lastMode = currentMode
    }
}
