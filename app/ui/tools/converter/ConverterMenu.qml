import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines
import "../../common"

Rectangle {
    id: side_bar
    color: palette.dark
    width: Defines.SIDEBAR_WIDTH

    signal openEditor

    ProjectComboBoxWithEditorButton {
        id: top_bar

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        model: converter_tool ? converter_tool.projects : []
        emptyString: converter_tool
                     && converter_tool.isLoading ? qsTr("Loading ...") : qsTr(
                                                       "No Units")

        onCurrentIndexChanged: function (index) {
            if (!converter_tool || converter_tool.projects.length < 1)
                return

            converter_tool.currentProject = converter_tool.projects[index]
        }

        onEditorButtonClicked: openEditor()
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

                model: converter_tool
                       && converter_tool.currentProject ? converter_tool.currentProject.categories : []

                CustomButton {
                    buttonText: qsTranslate("Units", modelData.name)

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: {
                        if (!converter_tool)
                            return

                        converter_tool.currentCategory = modelData
                    }
                }
            }
        }
    }
}
