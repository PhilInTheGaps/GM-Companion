import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root
    property int mode: -1
    property string element_name: ""

    property string modeString: {
        switch (mode) {
        case 0:
            qsTr("project")
            break
        case 1:
            qsTr("category")
            break
        case 2:
            qsTr("scenario")
            break
        case 3:
            qsTr("subscenario")
            break
        default:
            qsTr("UNKNOWN")
        }
    }

    margins: 0
    title: ""

    contentItem: Column {
        Text {
            text: qsTr(
                      "Delete " + modeString + (element_name != "" ? " " + element_name : "") + "?")
        }

        Text {
            text: qsTr("(This can not be undone!)")
        }
    }

    standardButtons: Dialog.Yes | Dialog.No

    onAccepted: {
        switch (root.mode) {
        case 0:
            audio_editor.deleteProject()
            break
        case 1:
            audio_editor.deleteCategory()
            break
        case 2:
            audio_editor.deleteScenario()
            break
        case 3:
            audio_editor.deleteSubScenario(element_name)
            break
        default:
            console.log("Error: Delete dialog in undefined mode")
        }

        element_name = ""
        mode = -1
    }
}
