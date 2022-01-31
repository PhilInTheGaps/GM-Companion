import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

import "../.."
import "../../../../../colors.js" as Colors
import "../../../../../defines.js" as Defines

Item {
    id: root
    anchors.left: parent.left
    anchors.right: parent.right
    height: Defines.TOOLBAR_HEIGHT

    Connections {
        target: audio_editor

        function onCurrentElementChanged() {
            element_icon_field.text = audio_editor
                    && audio_editor.currentElement ? audio_editor.currentElement.thumbnail.relativeUrl : ""
        }
    }

    Connections {
        target: unsplash_dialog
        function onAccepted() {
            element_icon_field.text = unsplash_dialog.imageSource
        }
    }

    Rectangle {
        id: image_preview
        visible: element_icon_image.status == Image.Ready
        border.color: Colors.border
        border.width: 1
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: height

        Image {
            id: element_icon_image
            anchors.fill: parent
            anchors.margins: 1
            source: audio_editor
                    && audio_editor.currentElement ? audio_editor.currentElement.thumbnail.absoluteUrl : ""
        }

        Text {
            anchors.fill: parent
            anchors.margins: 10
            text: FontAwesome.expand
            font.family: FontAwesome.familySolid
            font.pixelSize: height
            visible: element_icon_mouse_area.containsMouse
            color: "white"
        }

        MouseArea {
            id: element_icon_mouse_area
            anchors.fill: parent
            onClicked: large_image_dialog.open()
            hoverEnabled: true
        }
    }

    TextField {
        id: element_icon_field

        property bool save: false
        anchors.left: image_preview.visible ? image_preview.right : parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        rightPadding: unsplash_finder.width + icon_finder.width + 10

        selectByMouse: true
        placeholderText: {
            qsTr("Icon Path (Leave empty for default icon)")
        }

        text: audio_editor
              && audio_editor.currentElement ? audio_editor.currentElement.thumbnail.relativeUrl : ""

        onTextChanged: {
            if (save && audio_editor && audio_editor.currentElement) {
                audio_editor.currentElement.thumbnail.relativeUrl = text
            }
        }

        Connections {
            target: audio_editor
            function onCurrentElementChanged() {
                element_icon_field.save = false
                element_icon_field.text = audio_editor
                        && audio_editor.currentElement ? audio_editor.currentElement.thumbnail.relativeUrl : ""
                element_icon_field.save = true
            }
        }
    }

    CustomToolBarButton {
        id: unsplash_finder

        anchors.right: icon_finder.left
        anchors.margins: 0

        iconText: FontAwesome.search
        iconFont: FontAwesome.familySolid
        toolTipText: qsTr("Select icon from unsplash.com")
        pointSize: 12

        onClicked: {
            unsplash_dialog.open()
        }
    }

    IconFinder {
        id: icon_finder
        anchors.right: parent.right
        text_field: element_icon_field
    }
}
