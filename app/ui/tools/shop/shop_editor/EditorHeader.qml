import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts

CustomToolBar {
    id: root

    enableBack: true
    onBackClicked: backToViewer()

    enableAdd: true
    onAddClicked: new_thing_dialog.open()

    enableSave: true

    isSaved: shop_tool && shop_tool.editor ? shop_tool.editor.isSaved : true
    onSaveClicked: shop_tool.editor.save()

    Row {
        id: header_row_left
        anchors.left: button_row.right
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

            model: shop_tool
                   && shop_tool.editor ? shop_tool.editor.projects : []
            emptyString: shop_tool && shop_tool.editor
                         && shop_tool.editor.isLoading ? qsTr("Loading ...") : qsTr(
                                                             "No Projects")

            onCurrentIndexChanged: {
                if (!shop_tool || !shop_tool.editor
                        || shop_tool.editor.projects.length < 1)
                    return

                shop_tool.editor.currentProject = shop_tool.editor.projects[currentIndex]
            }
        }

        // Categories
        CustomToolBarComboBox {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 150
            textRole: "name"

            model: shop_tool && shop_tool.editor
                   && shop_tool.editor.currentProject ? shop_tool.editor.currentProject.categories : []
            emptyString: shop_tool && shop_tool.editor
                         && shop_tool.editor.currentProject
                         && shop_tool.editor.isLoading ? qsTr("Loading ...") : qsTr(
                                                             "No Categories")

            onCurrentIndexChanged: {
                if (!shop_tool || !shop_tool.editor
                        || !shop_tool.editor.currentProject)
                    return

                if (shop_tool.editor.currentProject.categories.length < 1)
                    return

                shop_tool.editor.currentProject.currentCategory
                        = shop_tool.editor.currentProject.categories[currentIndex]
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
            iconText: FontAwesome.penToSquare
            onClicked: switchToItemEditor()
        }

        // Item Groups
        CustomToolBarComboBox {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            textRole: "name"

            model: shop_tool
                   && shop_tool.editor ? shop_tool.editor.itemGroups : []
            emptyString: shop_tool && shop_tool.editor
                         && shop_tool.editor.isLoading ? qsTr("Loading ...") : qsTr(
                                                             "No Items")

            onCurrentIndexChanged: {
                if (!shop_tool || !shop_tool.editor
                        || shop_tool.editor.itemGroups.length < 1)
                    return

                shop_tool.editor.currentItemGroup = shop_tool.editor.itemGroups[currentIndex]
            }
        }
    }
}
