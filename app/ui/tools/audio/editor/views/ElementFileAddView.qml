import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import ".."
import "../../../../sizes.js" as Sizes
import "../../../../colors.js" as Colors

Rectangle {
    id: root

    property int mode: 0
    color: "transparent"

    function setMode(mode) {
        if (root.mode !== mode) {
            url_text_field.clear()
        }

        root.mode = mode
    }

    Connections {
        target: audio_editor

        function onCurrentElementChanged() {
            setMode(0)
        }
    }

    ToolBar {
        id: control
        anchors.left: parent.left
        anchors.right: parent.right

        Row {
            property var itemWidth: (width) / (spotify_button.visible ? 3 : 2)
            anchors.fill: parent

            CustomButton {
                id: folder_button
                iconText: FontAwesome.folder
                buttonText: qsTr("Files")

                usesFixedWidth: true
                width: parent.itemWidth
                padding: 0

                pointSize: 12
                centering: true
                transparentBackground: true

                onClicked: setMode(0)
            }

            CustomButton {
                id: spotify_button
                visible: audio_editor && audio_editor.currentElement
                         && audio_editor.currentElement.type === 0

                iconText: FontAwesome.spotify
                iconFont: FontAwesome.familyBrands
                buttonText: qsTr("Spotify")

                usesFixedWidth: true
                width: parent.itemWidth
                padding: 0

                pointSize: 12
                centering: true
                transparentBackground: true

                onClicked: setMode(1)
            }

            CustomButton {
                id: web_button
                iconText: FontAwesome.globe
                buttonText: qsTr("Web")

                usesFixedWidth: true
                width: parent.itemWidth
                padding: 0

                pointSize: 12
                centering: true
                transparentBackground: true

                onClicked: setMode(2)
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

        height: Sizes.toolbarHeight
        color: Colors.alternateBase

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
                audio_editor.addUrl(url_text_field.text, root.mode)
            }
        }
    }

    Rectangle {
        id: youtube_adder
        visible: root.mode > 1 && audio_editor && audio_editor.currentElement
                 && audio_editor.currentElement.type < 2
        anchors.topMargin: 5
        anchors.top: url_adder.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        height: Sizes.toolbarHeight
        color: Colors.alternateBase

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
                audio_editor.addYtUrl(youtube_text_field.text)
            }
        }
    }
}
