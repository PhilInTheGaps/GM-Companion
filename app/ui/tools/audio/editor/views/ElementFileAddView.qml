import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import ".."
import "../../../../defines.js" as Defines

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
        color: palette.alternateBase
        height: Defines.TOOLBAR_HEIGHT

        Row {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5

            CustomToolBarButton {
                id: folder_button
                iconText: FontAwesome.folder
                buttonText: qsTr("Files")
                onClicked: root.mode = 0
                pointSize: 12
                usesFixedWidth: false
            }

            CustomToolBarButton {
                id: spotify_button
                visible: audio_editor.type === 0
                iconText: FontAwesome.spotify
                iconFont: FontAwesome.familyBrands
                buttonText: qsTr("Spotify")
                pointSize: 12
                usesFixedWidth: false
                onClicked: {
                    if (root.mode != 1)
                        url_text_field.clear()
                    root.mode = 1
                }
            }

            CustomToolBarButton {
                id: web_button
                iconText: FontAwesome.globe
                buttonText: qsTr("Web")
                pointSize: 12
                usesFixedWidth: false
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

        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase

        CustomToolBarButton {
            id: url_adder_icon
            anchors.left: parent.left
            anchors.leftMargin: 10
            enabled: false

            iconText: root.mode === 1 ? FontAwesome.spotify : FontAwesome.globe
            iconFont: root.mode === 1 ? FontAwesome.familyBrands : FontAwesome.familySolid
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

        CustomToolBarButton {
            id: url_adder_button
            anchors.right: parent.right
            anchors.rightMargin: 10

            iconText: FontAwesome.plus

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

        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase

        CustomToolBarButton {
            id: youtube_adder_icon
            anchors.left: parent.left
            anchors.leftMargin: 10
            enabled: false

            iconText: FontAwesome.youtube
            iconFont: FontAwesome.familyBrands
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

        CustomToolBarButton {
            id: youtube_adder_button
            anchors.right: parent.right
            anchors.rightMargin: 10

            iconText: FontAwesome.plus

            onClicked: {
                audio_editor.addYtUrl(audio_editor.name, audio_editor.type,
                                      youtube_text_field.text)
            }
        }
    }
}
