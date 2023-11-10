import QtQuick
import CustomComponents
import IconFonts
import src
import "../../.."

CustomToolBar {
    id: root

    required property NewThingDialog newThingDialog

    signal backToViewer
    signal switchToItemEditor

    enableBack: true
    onBackClicked: root.backToViewer()

    enableAdd: true
    onAddClicked: root.newThingDialog.open()

    enableSave: true

    isSaved: ShopTool.editor ? ShopTool.editor.isSaved : true
    onSaveClicked: ShopTool.editor.save()

    Row {
        id: header_row_left
        anchors.left: root.button_row.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        padding: 10
        spacing: 5

        // Projects
        CustomToolBarComboBox {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150
            textRole: "name"

            model: ShopTool.editor ? ShopTool.editor.projects : []
            emptyString: ShopTool.editor && ShopTool.editor.isLoading ? qsTr("Loading ...") : qsTr("No Projects")

            onCurrentIndexChanged: {
                if (!ShopTool.editor || ShopTool.editor.projects.length < 1)
                    return;
                ShopTool.editor.currentProject = ShopTool.editor.projects[currentIndex];
            }
        }

        // Categories
        CustomToolBarComboBox {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150
            textRole: "name"

            model: ShopTool.editor && ShopTool.editor.currentProject ? ShopTool.editor.currentProject.categories : []
            emptyString: ShopTool.editor && ShopTool.editor.currentProject && ShopTool.editor.isLoading ? qsTr("Loading ...") : qsTr("No Categories")

            onCurrentIndexChanged: {
                if (!ShopTool.editor || !ShopTool.editor.currentProject)
                    return;
                if (ShopTool.editor.currentProject.categories.length < 1)
                    return;
                ShopTool.editor.currentProject.currentCategory = ShopTool.editor.currentProject.categories[currentIndex];
            }
        }
    }

    Row {
        id: header_row_right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        padding: 10
        spacing: 5

        layoutDirection: Qt.RightToLeft

        // Item Editor
        CustomToolBarButton {
            id: item_editor_button
            iconText: FontAwesome.penToSquare
            onClicked: root.switchToItemEditor()
        }

        // Item Groups
        CustomToolBarComboBox {
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            width: Sizes.sidebarWidth - item_editor_button.width - parent.spacing - parent.padding

            textRole: "name"

            model: ShopTool.editor ? ShopTool.editor.itemGroups : []
            emptyString: ShopTool.editor && ShopTool.editor.isLoading ? qsTr("Loading ...") : qsTr("No Items")

            onCurrentIndexChanged: {
                if (!ShopTool.editor || ShopTool.editor.itemGroups.length < 1)
                    return;
                ShopTool.editor.currentItemGroup = ShopTool.editor.itemGroups[currentIndex];
            }
        }
    }
}
