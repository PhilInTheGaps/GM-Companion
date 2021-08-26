import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import "../../../defines.js" as Defines

Page {
    id: root

    header: Row {
        spacing: 10
        height: Defines.TOOLBAR_HEIGHT

        Label {
            text: shop_tool.shopName
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
                text: shop_tool.shopOwner
                font.pointSize: 10
            }

            Label {
                id: shop_description_text
                text: shop_tool.shopDescription
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
        readonly property int descriptionColumnWidth: width - itemColumnWidth - priceColumnWidth
                                                      - columnPadding * 2 - columnSpacing * 2

        // Header
        Rectangle {
            id: table_header
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: Defines.TOOLBAR_HEIGHT - 10
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

            ScrollBar.vertical: ScrollBar {
                visible: list_view.contentHeight > list_view.height
            }

            model: shopItemModel

            delegate: Rectangle {
                height: delegate_row.height
                anchors.left: parent ? parent.left : undefined
                anchors.right: parent ? parent.right : undefined
                color: "transparent"

                Row {
                    id: delegate_row
                    padding: 5
                    leftPadding: main.columnPadding
                    rightPadding: main.columnPadding
                    spacing: main.columnSpacing

                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: description_label.height > Defines.TOOLBAR_HEIGHT
                            - 10 ? description_label.height : Defines.TOOLBAR_HEIGHT - 10

                    Label {
                        text: modelData.name
                        width: main.itemColumnWidth
                        clip: true
                        elide: Text.ElideRight
                        anchors.top: parent.top
                        topPadding: 5
                        font.pointSize: 12
                        font.bold: true
                    }

                    Label {
                        text: modelData.price
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
                        text: modelData.description
                        topPadding: 5
                        font.pointSize: 12
                        width: main.descriptionColumnWidth
                        anchors.top: parent.top
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
}
