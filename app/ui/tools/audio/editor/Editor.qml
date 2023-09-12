pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import src
import "../audio_exporter"
import "./dialogs"
import "./views"
import "../../.."
import "../../../common"

Page {
    id: editor_root

    signal projectAdded
    signal backToTool

    Component.onCompleted: AudioTool.editor.loadData()

    header: EditorHeader {
        deleteDialog: delete_dialog
        newThingDialog: new_thing_dialog
        renameDialog: rename_dialog
        exporterDialog: audio_exporter_dialog

        onBackClicked: {
            editor_root.backToTool()
        }
    }

    Connections {
        target: AudioTool.editor

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
        x: (parent.width - width) / 2
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
            source: AudioTool.editor.currentElement ? AudioTool.editor.currentElement.thumbnail.absoluteUrl : ""

            fillMode: Image.PreserveAspectFit
        }
    }

    EditorRenameDialog {
        id: rename_dialog
    }

    EditorDeleteDialog {
        id: delete_dialog
    }

    CustomFileDialog {
        id: audio_editor_file_dialog
    }

    EditorMainView {
        id: main_view
        anchors.fill: parent

        fileDialog: audio_editor_file_dialog // qmllint disable incompatible-type
        deleteDialog: delete_dialog
        unsplashDialog: unsplash_dialog
        largeImageDialog: large_image_dialog
    }

    Rectangle {
        id: info_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: Sizes.toolbarHeight
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
