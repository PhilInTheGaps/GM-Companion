import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import ".."
import "../../../.."

Item {
    id: root

    property int mode: 0

    function setMode(mode) {
        if (root.mode !== mode) {
            url_text_field.clear()
        }

        root.mode = mode
    }

    Connections {
        target: AudioTool.editor

        function onCurrentElementChanged() {
            root.setMode(0)
        }
    }

    ToolBar {
        id: control
        anchors.left: parent.left
        anchors.right: parent.right

        Row {
            property int itemWidth: (width) / (spotify_button.visible ? 3 : 2)
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

                onClicked: root.setMode(0)
            }

            CustomButton {
                id: spotify_button
                visible: AudioTool.editor.currentElement
                         && AudioTool.editor.currentElement.type === 0

                iconText: FontAwesome.spotify
                iconFont: FontAwesome.fontBrands
                buttonText: qsTr("Spotify")

                usesFixedWidth: true
                width: parent.itemWidth
                padding: 0

                pointSize: 12
                centering: true
                transparentBackground: true

                onClicked: root.setMode(1)
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

                onClicked: root.setMode(2)
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
        color: palette.alternateBase

        CustomToolBarButton {
            id: url_adder_icon
            anchors.left: parent.left
            anchors.leftMargin: 10
            enabled: false

            iconText: root.mode === 1 ? FontAwesome.spotify : FontAwesome.globe
            iconFont: root.mode === 1 ? FontAwesome.fontBrands : FontAwesome.fontSolid
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
                AudioTool.editor.addUrl(url_text_field.text, root.mode)
            }
        }
    }

//    Rectangle {
//        id: youtube_adder
//        visible: root.mode > 1 && AudioTool.editor.currentElement
//                 && AudioTool.editor.currentElement.type < 2
//        anchors.topMargin: 5
//        anchors.top: url_adder.bottom
//        anchors.right: parent.right
//        anchors.left: parent.left

//        height: Sizes.toolbarHeight
//        color: palette.alternateBase

//        CustomToolBarButton {
//            id: youtube_adder_icon
//            anchors.left: parent.left
//            anchors.leftMargin: 10
//            enabled: false

//            iconText: FontAwesome.youtube
//            iconFont: FontAwesome.fontBrands
//        }

//        TextField {
//            id: youtube_text_field
//            anchors.left: youtube_adder_icon.right
//            anchors.right: youtube_adder_button.left
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            anchors.leftMargin: 10
//            anchors.rightMargin: 10

//            selectByMouse: true
//            placeholderText: qsTr("YouTube URL")
//        }

//        CustomToolBarButton {
//            id: youtube_adder_button
//            anchors.right: parent.right
//            anchors.rightMargin: 10

//            iconText: FontAwesome.plus

//            onClicked: {
//                AudioTool.editor.addYtUrl(youtube_text_field.text)
//            }
//        }
//    }
}
