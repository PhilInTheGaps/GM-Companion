import QtQuick
import QtQuick.Controls
import CustomComponents
import src
import "./buttons"
import "../.."
import "../../common"

Rectangle {
    id: left_menu
    color: palette.dark

    signal editorButtonClicked

    ProjectComboBoxWithEditorButton
    {
        id: top_bar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        model: AudioTool.projects
        textRole: "name"

        emptyString: AudioTool.isLoading ? qsTr("Loading ...") : qsTr(
                                                   "No Projects")

        onCurrentTextChanged: {
            AudioTool.setCurrentProject(projectIndex)
        }

        onEditorButtonClicked: left_menu.editorButtonClicked()
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
