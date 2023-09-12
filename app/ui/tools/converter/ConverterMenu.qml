import QtQuick
import QtQuick.Controls
import CustomComponents
import src
import "../.."
import "../../common"

Rectangle {
    id: root

    signal openEditor

    color: palette.dark
    width: Sizes.sidebarWidth

    ProjectComboBoxWithEditorButton {
        id: top_bar

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        model: ConverterTool.projects
        emptyString: ConverterTool.isLoading ? qsTr("Loading ...") : qsTr(
                                                       "No Units")

        onCurrentIndexChanged: function (index) {
            if (ConverterTool.projects.length < 1)
                return

            ConverterTool.currentProject = ConverterTool.projects[index]
        }

        onEditorButtonClicked: root.openEditor()
    }

    ScrollView {
        id: type_view

        anchors.top: top_bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5

        clip: true
        contentWidth: -1
        contentHeight: type_column.implicitHeight

        Column {
            id: type_column
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                id: types_repeater

                model: ConverterTool.currentProject ? ConverterTool.currentProject.categories : []

                CustomButton {
                    required property ConverterCategory modelData

                    buttonText: qsTranslate("Units", modelData.name)

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: {
                        ConverterTool.currentCategory = modelData
                    }
                }
            }
        }
    }
}
