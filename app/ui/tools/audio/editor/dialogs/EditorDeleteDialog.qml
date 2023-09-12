import QtQuick
import QtQuick.Controls
import src

Dialog {
    id: root
    property int mode: -1
    property var element

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

    modal: true
    margins: 0
    title: ""

    contentItem: Column {
        Label {
            text: qsTr("Delete" + " " + root.modeString + " "
                       + (root.element ? root.element.name : "UNKNOWN") + "?")
        }

        Label {
            text: qsTr("(This can not be undone!)")
        }
    }

    standardButtons: Dialog.Yes | Dialog.No

    onAccepted: {
        switch (mode) {
        case 0:
            AudioTool.editor.deleteProject()
            break
        case 1:
            AudioTool.editor.deleteCategory()
            break
        case 2:
            AudioTool.editor.deleteScenario()
            break
        case 3:
            AudioTool.editor.deleteSubScenario(element)
            break
        }

        element = undefined
        mode = -1
    }
}
