import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root
    property string origName: ""
    property int mode: -1

    margins: 0
    title: ""

    contentItem: Column {
        TextField {
            id: field
            text: origName
            selectByMouse: true
            onAccepted: root.accept()
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onOpened: field.forceActiveFocus()

    onAccepted: {
        switch (root.mode) {
        case 0:
            audio_editor.renameProject(field.text)
            break
        case 1:
            audio_editor.renameCategory(field.text)
            break
        case 2:
            audio_editor.renameScenario(field.text)
            break
        default:
            console.log("Error: Rename dialog in undefined mode")
        }
    }
}
