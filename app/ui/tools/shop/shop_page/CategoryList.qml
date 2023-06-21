import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../../defines.js" as Defines
import "../../../common"

Rectangle {
    id: left_menu
    color: palette.dark

    signal editorButtonClicked

    ProjectComboBoxWithEditorButton {
        id: top_bar

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: Defines.TOOLBAR_HEIGHT

        model: shop_tool ? shop_tool.projects : []
        emptyString: shop_tool
                     && shop_tool.isLoading ? qsTr("Loading ...") : qsTr(
                                                  "No Projects")

        onCurrentIndexChanged: {
            if (!shop_tool || shop_tool.projects.length < 1)
                return

            shop_tool.currentProject = shop_tool.projects[index]
        }

        onEditorButtonClicked: left_menu.editorButtonClicked()
    }

    ScrollView {
        anchors.top: top_bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5

        clip: true
        contentWidth: -1
        contentHeight: category_column.implicitHeight

        Column {
            id: category_column
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: shop_tool
                       && shop_tool.currentProject ? shop_tool.currentProject.categories : []

                CustomButton {
                    buttonText: modelData.name

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: shop_tool.currentProject.currentCategory = modelData
                }
            }
        }
    }
}
