import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts

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
        if (!converter_editor) {
            canAccept = false
            return
        }

        switch (currentMode) {
        case ConverterEditorNewThingDialog.Mode.Project:
            canAccept = true
            break
        case ConverterEditorNewThingDialog.Mode.Category:
            canAccept = converter_editor.currentProject
            error_label.text = qsTr("No Project!")
            break
        case ConverterEditorNewThingDialog.Mode.Unit:
            canAccept = converter_editor.currentCategory
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
                updateCanAccept()
                name_field.forceActiveFocus()
            }
        }

        TextField {
            id: name_field
            placeholderText: qsTr("Element Name")
            selectByMouse: true
            enabled: canAccept
            visible: canAccept
            onAccepted: {
                if (currentMode !== ConverterEditorNewThingDialog.Mode.Unit) {
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
            enabled: canAccept
            visible: canAccept
                     && currentMode === ConverterEditorNewThingDialog.Mode.Unit

            validator: DoubleValidator {
                notation: DoubleValidator.ScientificNotation
            }
        }

        Label {
            id: error_label
            visible: !canAccept

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
        if (name_field.text != "" && canAccept) {

            switch (currentMode) {
            case ConverterEditorNewThingDialog.Mode.Project:
                converter_editor.createProject(name_field.text)
                break
            case ConverterEditorNewThingDialog.Mode.Category:
                converter_editor.createCategory(name_field.text)
                break
            case ConverterEditorNewThingDialog.Mode.Unit:
                converter_editor.createUnit(name_field.text, value_field.text)
                break
            }
        }

        name_field.clear()
    }

    onClosed: {
        lastMode = currentMode
    }
}
