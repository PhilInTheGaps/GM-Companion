import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../../.."

Page {
    id: root

    signal backToShopEditor

    Connections {
        target: ShopTool && ShopTool.editor && ShopTool.editor.itemEditor ? ShopTool.editor.itemEditor : undefined

        function onShowInfoBar(message) {
            info_text.text = message;
            info_bar.visible = true;
            info_bar_timer.start();
        }
    }

    header: CustomToolBar {
        id: header_bar

        enableBack: true
        onBackClicked: root.backToShopEditor()

        enableSave: true
        isSaved: ShopTool && ShopTool.editor && ShopTool.editor.itemEditor ? ShopTool.editor.itemEditor.isSaved : true
        onSaveClicked: ShopTool.editor.itemEditor.save()
    }

    contentItem: SplitView {
        id: split_view
        orientation: Qt.Horizontal

        ItemDetails {
            id: item_details
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            SplitView.minimumWidth: 160
            SplitView.preferredWidth: 200
        }

        Item {
            id: right_item
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 300
            SplitView.fillWidth: true

            Rectangle {
                id: item_header
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 5

                height: Sizes.toolbarHeight
                color: palette.button

                Row {
                    anchors.fill: parent
                    padding: 5
                    leftPadding: 10
                    rightPadding: 10
                    spacing: 10

                    Label {
                        text: qsTr("Name")
                        font.pointSize: 12
                        font.bold: true
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Label {
                        text: qsTr("Price")
                        font.pointSize: 12
                        font.bold: true
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        text: qsTr("Category")
                        font.pointSize: 12
                        font.bold: true
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Label {
                        text: qsTr("Description")
                        font.pointSize: 12
                        font.bold: true
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 2
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            ListView {
                id: item_table_view
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: item_header.bottom
                anchors.bottom: parent.bottom
                anchors.margins: 5

                clip: true
                spacing: 0

                ScrollBar.vertical: ScrollBar {
                }

                model: ShopTool.editor && ShopTool.editor.itemEditor ? ShopTool.editor.itemEditor.itemModel : []

                delegate: Rectangle {
                    id: item_rect

                    required property string name
                    required property string price
                    required property string category
                    required property string description
                    required property int index

                    height: delegate_row.height
                    anchors.left: parent ? parent.left : undefined
                    anchors.right: parent ? parent.right : undefined

                    color: "transparent"
                    border.color: palette.dark
                    border.width: mouse_area.containsMouse ? 1 : 0

                    ToolTip.text: name + ", " + price + ", " + category + "\n" + description
                    ToolTip.delay: 1000
                    ToolTip.visible: mouse_area.containsMouse

                    MouseArea {
                        id: mouse_area
                        anchors.fill: parent
                        hoverEnabled: true

                        z: 2
                        onClicked: mouse => mouse.accepted = false
                        onPressed: mouse => mouse.accepted = false
                        onReleased: mouse => mouse.accepted = false
                        onDoubleClicked: mouse => mouse.accepted = false
                        onPositionChanged: mouse => mouse.accepted = false
                        onPressAndHold: mouse => mouse.accepted = false
                    }

                    Row {
                        id: delegate_row
                        padding: 5
                        leftPadding: 10
                        rightPadding: 10
                        spacing: 10
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 30

                        Label {
                            text: item_rect.name
                            width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                            clip: true
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 12
                        }

                        Label {
                            text: item_rect.price
                            width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                            clip: true
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignRight
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 12
                        }

                        Label {
                            text: item_rect.category
                            width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                            clip: true
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 12
                        }

                        Label {
                            text: item_rect.description
                            width: (parent.width - x - delete_button.width - parent.spacing - parent.rightPadding)
                            clip: true
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 12
                        }

                        Button {
                            id: delete_button
                            height: parent.height - parent.topPadding * 2
                            width: height
                            hoverEnabled: true
                            visible: mouse_area.containsMouse

                            background: Rectangle {
                                color: "transparent"
                            }

                            Text {
                                text: FontAwesome.xmark
                                font.family: FontAwesome.fontSolid.family
                                font.styleName: FontAwesome.fontSolid.styleName
                                font.pointSize: 20
                                anchors.centerIn: parent
                                color: parent.pressed ? "grey" : (parent.hovered ? "lightgrey" : palette.text)
                            }

                            onClicked: ShopTool.editor.itemEditor.deleteItem(item_rect.index)
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
