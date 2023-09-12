import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "./buttons"
import "../.."

Rectangle {
    id: left_menu
    color: palette.dark

    signal editorButtonClicked

    Item {
        id: top_bar

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: Sizes.toolbarHeight

        anchors.leftMargin: 5
        anchors.rightMargin: 5

        // Project ComboBox
        CustomToolBarComboBox {
            id: audio_project_combo_box

            anchors.left: parent.left
            anchors.right: editor_button.left

            width: parent.width - editor_button.width
            model: AudioTool.projects
            textRole: "name"
            emptyString: AudioTool.isLoading ? qsTr("Loading ...") : qsTr(
                                                       "No Projects")

            onCurrentTextChanged: {
                AudioTool.setCurrentProject(currentIndex)
            }
        }

        // Open Editor Button
        CustomToolBarButton {
            id: editor_button
            iconText: FontAwesome.penToSquare

            anchors.right: parent.right
            anchors.topMargin: 8
            anchors.bottomMargin: 8

            onClicked: left_menu.editorButtonClicked()
        }
    }

    ScrollView {
        id: audio_project_menu

        anchors.top: top_bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: sound_column.top
        anchors.margins: 5

        clip: true
        contentWidth: -1
        contentHeight: audio_project_structure.implicitHeight

        Column {
            id: audio_project_structure
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                id: category_repeater

                model: AudioTool.currentProject ? AudioTool.currentProject.categories : []

                CustomButton {
                    required property AudioCategory modelData

                    buttonText: modelData.name // qmllint disable missing-property

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: {
                        AudioTool.currentProject.currentCategory = modelData
                    }
                }
            }
        }
    }

    Column {
        id: sound_column
        anchors.bottom: cover_image.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 5
        bottomPadding: 5

        Repeater {
            id: sound_repeater
            model: AudioTool.soundController.activeElements

            SoundButton {
                required property AudioElement modelData

                elementName: modelData.name // qmllint disable missing-property
                elementThumbnail: modelData.thumbnail
                anchors.left: parent.left
                anchors.right: parent.right
            }
        }
    }

    Image {
        id: cover_image
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        visible: source !== ""
        sourceSize.width: width
        source: AudioTool.metaData.cover
    }
}
