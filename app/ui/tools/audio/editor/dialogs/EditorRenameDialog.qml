import QtQuick
import QtQuick.Controls
import src

Dialog {
    id: root

    property string originalName: ""
    property int mode: -1

    modal: true
    margins: 0
    title: ""

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    contentItem: Column {
        TextField {
            id: field
            text: root.originalName
            selectByMouse: true
            onAccepted: root.accept()
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onOpened: field.forceActiveFocus()

    onAccepted: {
        switch (root.mode) {
        case 0:
            AudioTool.editor.renameProject(field.text)
            break
        case 1:
            AudioTool.editor.renameCategory(field.text)
            break
        case 2:
            AudioTool.editor.renameScenario(field.text)
            break
        default:
            console.log("Error: Rename dialog in undefined mode")
        }
    }
}
