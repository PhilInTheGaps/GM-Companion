pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import src
import "../../.."

Page {
    id: root

    signal backToViewer
    signal switchToItemEditor

    Component.onCompleted: ShopTool.editor.loadData()

    Connections {
        target: ShopTool.editor

        function onShowInfoBar(message) {
            info_text.text = message;
            info_bar.visible = true;
            info_bar_timer.start();
        }
    }

    NewThingDialog {
        id: new_thing_dialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    header: ShopEditorHeader {
        id: tool_bar
        newThingDialog: new_thing_dialog
        onBackToViewer: root.backToViewer()
        onSwitchToItemEditor: root.switchToItemEditor()
    }

    contentItem: SplitView {
        id: split_view
        orientation: Qt.Horizontal

        EditorShopList {
            id: shop_list
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: Sizes.sidebarWidth
        }

        ShopInfo {
            id: shop_info
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 5

            SplitView.minimumWidth: 300
            SplitView.fillWidth: true
        }

        ItemList {
            id: item_list
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 160
            SplitView.preferredWidth: Sizes.sidebarWidth * 2
        }

        // Item Categories
        Rectangle {
            id: item_right_bar
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 50
            SplitView.preferredWidth: Sizes.sidebarWidth

            color: palette.dark

            CheckBox {
                id: item_category_select_bar
                text: qsTr("All Categories")
                checked: true

                height: Sizes.toolbarHeight
                anchors.left: parent.left
                anchors.right: parent.right

                Rectangle {
                    color: palette.button
                    height: 1
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                onClicked: ShopTool.editor.enableAllItemCategories(checked)
            }

            Flickable {
                id: item_category_flickable

                anchors.top: item_category_select_bar.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.topMargin: 5
                clip: true
                contentHeight: item_category_column.implicitHeight
                ScrollBar.vertical: ScrollBar {
                    visible: item_category_flickable.contentHeight > item_category_flickable.height
                }

                Column {
                    id: item_category_column
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Repeater {
                        id: item_category_repeater

                        model: ShopTool.editor && ShopTool.editor.currentItemGroup ? ShopTool.editor && ShopTool.editor.currentItemGroup.categories : []
                        CheckBox {
                            id: category_checkbox

                            required property string modelData

                            anchors.left: parent.left
                            anchors.right: parent.right

                            text: modelData

                            onClicked: ShopTool.editor.setItemCategoryEnabled(modelData, checked)

                            Component.onCompleted: {
                                checked = ShopTool.editor ? ShopTool.editor.isItemCategoryEnabled(modelData) : false;
                            }

                            Connections {
                                target: item_category_select_bar

                                function onCheckedChanged() {
                                    category_checkbox.checked = item_category_select_bar.checked;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: info_bar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: Sizes.toolbarHeight
        color: palette.alternateBase
        visible: false

        Timer {
            id: info_bar_timer
            interval: 2000

            onTriggered: {
                info_bar.visible = false;
                stop();
            }
        }

        Label {
            id: info_text
            anchors.fill: parent
            anchors.margins: 10
            font.pixelSize: height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
