import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

CustomToolBar {
    id: root

    enableBack: true
    onBackClicked: backToViewer()

    enableAdd: true
    onAddClicked: new_thing_dialog.open()

    enableSave: true
    isSaved: shop_editor.isSaved
    onSaveClicked: shop_editor.save()

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
            model: shop_editor.projectNames
            onCurrentIndexChanged: shop_editor.setCurrentProject(currentIndex)
        }

        // Categories
        CustomToolBarComboBox {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            model: shop_editor.categoryNames
            onCurrentIndexChanged: shop_editor.setCurrentCategory(currentIndex)
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
            iconText: FontAwesome.edit
            onClicked: switchToItemEditor()
        }

        // Item Groups
        CustomToolBarComboBox {
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            model: shop_editor.itemGroups
            onCurrentIndexChanged: shop_editor.setCurrentItemGroup(currentIndex)
        }
    }
}
