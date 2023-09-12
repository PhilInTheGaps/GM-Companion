import QtQuick
import QtQuick.Controls
import CustomComponents
import src
import "../../.."
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
        height: Sizes.toolbarHeight

        model: ShopTool.projects
        emptyString: ShopTool.isLoading ? qsTr("Loading ...") : qsTr(
                                                  "No Projects")

        onCurrentIndexChanged: function (index) {
            if (!ShopTool || ShopTool.projects.length < 1)
                return

            ShopTool.currentProject = ShopTool.projects[index]
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
                model: ShopTool.currentProject ? ShopTool.currentProject.categories : []

                CustomButton {
                    required property ShopCategory modelData

                    buttonText: modelData.name // qmllint disable missing-property

                    anchors.left: parent.left
                    anchors.right: parent.right

                    onClicked: ShopTool.currentProject.currentCategory = modelData
                }
            }
        }
    }
}
