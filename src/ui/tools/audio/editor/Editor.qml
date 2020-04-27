import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

import FontAwesome 2.0
import "../buttons"
import "../audio_exporter"
import "./views"
import "../../../defines.js" as Defines

Page {
    id: editor_root

    signal backToTool
    signal projectAdded

    Component.onCompleted: audio_editor.loadData()

    header: EditorHeader {}

    Connections {
        target: audio_editor

        onShowInfoBar: {
            info_text.text = message
            info_bar.visible = true
            info_bar_timer.start()
        }
    }

    // New Project, Category, Scenario, Element...
    EditorNewThingDialog {
        id: new_thing_dialog
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
            source: if (audio_editor.icon.startsWith("http")
                            || audio_editor.icon.startsWith("data:")) {
                        audio_editor.icon
                    } else {
                        (platform.isWindows ? "file:///" : "file://") + audio_editor.resourcesPath(
                                    ) + audio_editor.icon
                    }

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

    Item {
        id: main_item
        anchors.fill: parent

        Item {
            id: left_item
            width: 220

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            EditorElementColumn {
                id: element_column
            }
        }

        EditorFileListView {
            id: mid_item

            anchors.left: left_item.right
            anchors.right: right_item.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            anchors.topMargin: 5
            anchors.bottomMargin: 5
        }

        Item {
            id: right_item
            width: 250

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.bottomMargin: 5

            EditorRightColumn {
                id: right_column
                anchors.fill: parent
            }
        }
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
