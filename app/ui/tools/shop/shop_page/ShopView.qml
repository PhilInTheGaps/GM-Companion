pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import src
import "../../.."

Page {
    id: root

    property var shop: undefined

    leftPadding: 5
    rightPadding: 5

    header: Row {
        leftPadding: 5
        rightPadding: 5
        spacing: 10
        height: Sizes.toolbarHeight

        Label {
            text: root.shop ? root.shop.name : ""
            font.pixelSize: height - 20
            font.bold: true
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            verticalAlignment: Text.AlignVCenter
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            leftPadding: 10

            Label {
                id: shop_owner_text
                text: root.shop ? root.shop.owner : ""
                font.pointSize: 10
            }

            Label {
                id: shop_description_text
                text: root.shop ? root.shop.description : ""
                font.pointSize: 10
            }
        }
    }

    contentItem: Item {
        id: main
        readonly property int columnSpacing: 20
        readonly property int columnPadding: 10

        readonly property int itemColumnWidth: width / 5
        readonly property int priceColumnWidth: width / 5
        readonly property int descriptionColumnWidth: width - itemColumnWidth - priceColumnWidth - columnPadding * 2 - columnSpacing * 2

        // Header
        Rectangle {
            id: table_header
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: Sizes.toolbarHeight - 10
            color: palette.button

            Row {
                anchors.fill: parent
                padding: main.columnPadding
                spacing: main.columnSpacing

                Label {
                    text: qsTr("Item")
                    anchors.verticalCenter: parent.verticalCenter
                    width: main.itemColumnWidth
                    horizontalAlignment: Text.AlignLeft
                    font.pointSize: 12
                }

                Label {
                    text: qsTr("Price")
                    anchors.verticalCenter: parent.verticalCenter
                    width: main.priceColumnWidth
                    horizontalAlignment: Text.AlignRight
                    font.pointSize: 12
                }

                Label {
                    text: qsTr("Description")
                    anchors.verticalCenter: parent.verticalCenter
                    width: main.descriptionColumnWidth
                    horizontalAlignment: Text.AlignLeft
                    font.pointSize: 12
                }
            }
        }

        ListView {
            id: list_view
            anchors.top: table_header.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 5

            clip: true
            spacing: 0
            focus: true

            ScrollBar.vertical: ScrollBar {
                visible: list_view.contentHeight > list_view.height
            }

            model: ShopTool.itemModel

            delegate: Rectangle {
                id: item_rect

                required property int index
                required property string name
                required property string price
                required property string description

                height: delegate_row.height
                anchors.left: parent ? parent.left : undefined
                anchors.right: parent ? parent.right : undefined

                color: "transparent"
                border.color: ListView.isCurrentItem ? palette.button : palette.dark
                border.width: ListView.isCurrentItem || mouse_area.containsMouse ? 1 : 0

                Row {
                    id: delegate_row
                    padding: 5
                    leftPadding: main.columnPadding
                    rightPadding: main.columnPadding
                    spacing: main.columnSpacing

                    anchors.left: parent ? parent.left : undefined
                    anchors.right: parent ? parent.right : undefined
                    height: description_label.height > Sizes.toolbarHeight - 10 ? description_label.height : Sizes.toolbarHeight - 10

                    Label {
                        text: item_rect.name
                        width: main.itemColumnWidth
                        clip: true
                        elide: Text.ElideRight
                        anchors.top: parent.top
                        topPadding: 5
                        font.pointSize: 12
                        font.bold: true
                    }

                    Label {
                        text: item_rect.price
                        width: main.priceColumnWidth
                        clip: true
                        elide: Text.ElideRight
                        anchors.top: parent.top
                        topPadding: 5
                        font.pointSize: 12
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: description_label
                        text: item_rect.description
                        topPadding: 5
                        font.pointSize: 12
                        width: main.descriptionColumnWidth
                        anchors.top: parent.top
                        wrapMode: Text.WordWrap
                    }
                }

                MouseArea {
                    id: mouse_area
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: list_view.currentIndex = item_rect.index
                }
            }
        }
    }
}
