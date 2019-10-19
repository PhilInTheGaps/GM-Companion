import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

import "../../../../components"
import ".."

Rectangle {
    id: root

    property int mode: 0

    anchors.left: parent.left
    anchors.right: parent.right
    color: "transparent"

    Connections {
        target: audio_editor
        onCurrentElementChanged: {
            if (audio_editor.type > 0 && root.mode == 1) {
                root.mode = 0
            }
        }
    }

    Rectangle {
        id: control
        anchors.left: parent.left
        anchors.right: parent.right
        color: color_scheme.toolbarColor
        height: color_scheme.toolbarHeight

        Row {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            spacing: 15

            ToolBarIconButton {
                fa_icon: FontAwesome.folder
                button_text: qsTr("Files")
                onClicked: root.mode = 0
            }

            ToolBarIconButton {
                visible: audio_editor.type === 0
                fa_icon: FontAwesome.spotify
                fa_font: FontAwesome.familyBrands
                button_text: qsTr("Spotify")
                onClicked: {
                    if (root.mode != 1)
                        url_text_field.clear()
                    root.mode = 1
                }
            }

            ToolBarIconButton {
                fa_icon: FontAwesome.globe
                button_text: qsTr("Web")
                onClicked: {
                    if (root.mode != 2)
                        url_text_field.clear()
                    root.mode = 2
                }
            }
        }
    }

    EditorFileBrowser {
        id: file_browser
        visible: root.mode === 0
        anchors.topMargin: 5
        anchors.top: control.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
    }

    Rectangle {
        id: url_adder
        visible: root.mode > 0
        anchors.topMargin: 5
        anchors.top: control.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        height: color_scheme.toolbarHeight
        color: color_scheme.toolbarColor

        ToolBarIconButton {
            id: url_adder_icon
            anchors.left: parent.left
            anchors.leftMargin: 10
            enabled: false

            fa_icon: root.mode === 1 ? FontAwesome.spotify : FontAwesome.globe
            fa_font: root.mode === 1 ? FontAwesome.familyBrands : FontAwesome.familySolid
        }

        TextField {
            id: url_text_field
            anchors.left: url_adder_icon.right
            anchors.right: url_adder_button.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            selectByMouse: true
            placeholderText: root.mode === 1 ? qsTr("Spotify URI") : qsTr("URL")
        }

        ToolBarIconButton {
            id: url_adder_button
            anchors.right: parent.right
            anchors.rightMargin: 10

            fa_icon: FontAwesome.plus

            onClicked: {
                audio_editor.addUrl(audio_editor.name, audio_editor.type,
                                    url_text_field.text, root.mode)
            }
        }
    }

    Rectangle {
        id: youtube_adder
        visible: root.mode > 1 && audio_editor.type < 2
        anchors.topMargin: 5
        anchors.top: url_adder.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        height: color_scheme.toolbarHeight
        color: color_scheme.toolbarColor

        ToolBarIconButton {
            id: youtube_adder_icon
            anchors.left: parent.left
            anchors.leftMargin: 10
            enabled: false

            fa_icon: FontAwesome.youtube
            fa_font: FontAwesome.familyBrands
        }

        TextField {
            id: youtube_text_field
            anchors.left: youtube_adder_icon.right
            anchors.right: youtube_adder_button.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            selectByMouse: true
            placeholderText: qsTr("YouTube URL")
        }

        ToolBarIconButton {
            id: youtube_adder_button
            anchors.right: parent.right
            anchors.rightMargin: 10

            fa_icon: FontAwesome.plus

            onClicked: {
                audio_editor.addYtUrl(audio_editor.name, audio_editor.type,
                                      youtube_text_field.text)
            }
        }
    }
}
