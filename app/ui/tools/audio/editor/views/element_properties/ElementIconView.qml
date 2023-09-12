import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."
import "../../../../.."
import "../../../../../common"

Item {
    id: root

    required property CustomFileDialog fileDialog
    required property IconFinderUnsplash unsplashDialog
    required property Dialog largeImageDialog

    anchors.left: parent.left
    anchors.right: parent.right
    height: Sizes.toolbarHeight

    Connections {
        target: AudioTool.editor

        function onCurrentElementChanged() {
            element_icon_field.text = AudioTool.editor.currentElement
                    ? AudioTool.editor.currentElement.thumbnail.relativeUrl : ""
        }
    }

    Connections {
        target: root.unsplashDialog
        function onAccepted() {
            element_icon_field.text = root.unsplashDialog.imageSource
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
            source: AudioTool.editor.currentElement ? AudioTool.editor.currentElement.thumbnail.absoluteUrl : ""
        }

        Text {
            anchors.fill: parent
            anchors.margins: 10
            text: FontAwesome.expand
            font.family: FontAwesome.fontSolid.family
            font.styleName: FontAwesome.fontSolid.styleName
            font.pixelSize: height
            visible: element_icon_mouse_area.containsMouse
            color: "white"
        }

        MouseArea {
            id: element_icon_mouse_area
            anchors.fill: parent
            onClicked: root.largeImageDialog.open()
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

        text: AudioTool.editor.currentElement ? AudioTool.editor.currentElement.thumbnail.relativeUrl : ""

        onTextChanged: {
            if (save && AudioTool.editor.currentElement) {
                AudioTool.editor.currentElement.thumbnail.relativeUrl = text
            }
        }

        Connections {
            target: AudioTool.editor
            function onCurrentElementChanged() {
                element_icon_field.save = false
                element_icon_field.text = AudioTool.editor.currentElement ? AudioTool.editor.currentElement.thumbnail.relativeUrl : ""
                element_icon_field.save = true
            }
        }
    }

    CustomToolBarButton {
        id: unsplash_finder

        anchors.right: icon_finder.left
        anchors.margins: 0

        iconText: FontAwesome.magnifyingGlass
        iconFont: FontAwesome.fontSolid
        toolTipText: qsTr("Select icon from unsplash.com")
        pointSize: 12

        onClicked: {
            root.unsplashDialog.open()
        }
    }

    IconFinder {
        id: icon_finder
        anchors.right: parent.right
        textField: element_icon_field
        fileDialog: root.fileDialog // qmllint disable incompatible-type
    }
}
