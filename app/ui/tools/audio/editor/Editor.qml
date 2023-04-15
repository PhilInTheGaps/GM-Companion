import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "../audio_exporter"
import "./dialogs"
import "./views"
import "./views/main"
import "./views/element_properties"
import "../../../defines.js" as Defines
import "../../../common"

Page {
    id: editor_root

    signal projectAdded

    Component.onCompleted: audio_editor.loadData()

    header: EditorHeader {}

    Connections {
        target: audio_editor

        function onShowInfoBar(message) {
            info_text.text = message
            info_bar.visible = true
            info_bar_timer.start()
        }
    }

    // New Project, Category, Scenario, Element...
    EditorNewThingDialog {
        id: new_thing_dialog
        onOpenAddonDialog: {
            addon_dialog.open()
            close()
        }
    }

    AddonElementDialog {
        id: addon_dialog

        width: parent.width - 100
        height: parent.height - 50
        x: 50

        mode: new_thing_dialog.currentMode
        asSubscenario: new_thing_dialog.subscenarioChecked
        subscenarioIndex: new_thing_dialog.subscenarioIndex
    }

    IconFinderUnsplash {
        id: unsplash_dialog
        x: (parent.width - width - audio_stack.x) / 2
        y: (parent.height - height) / 2
    }

    AudioExporter {
        id: audio_exporter_dialog
        width: parent.width - 100
        height: parent.height - 100
        x: 50
        y: 50
    }

    Dialog {
        id: large_image_dialog
        modal: true

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        contentItem: Image {
            source: audio_editor
                    && audio_editor.currentElement ? audio_editor.currentElement.thumbnail.absoluteUrl : ""

            fillMode: Image.PreserveAspectFit
        }

        onOpened: console.log(contentItem.source)
    }

    EditorRenameDialog {
        id: rename_dialog
    }

    EditorDeleteDialog {
        id: delete_dialog
    }

    FileDialog {
        id: audio_editor_file_dialog
    }

    EditorMainView {
        id: main_view
        anchors.fill: parent
    }

    Rectangle {
        id: info_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase
        visible: false

        Timer {
            id: info_bar_timer
            interval: 2000

            onTriggered: {
                info_bar.visible = false
                stop()
            }
        }

        Label {
            id: info_text
            anchors.fill: parent
            anchors.margins: 10
            font.pixelSize: height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
