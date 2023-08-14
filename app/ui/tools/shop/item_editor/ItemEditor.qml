import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import IconFonts
import "../../../defines.js" as Defines

Page {
    id: editor

    signal backToShopEditor

    Connections {
        target: shop_tool && shop_tool.editor
                && shop_tool.editor.itemEditor ? shop_tool.editor.itemEditor : undefined

        function onShowInfoBar(message) {
            info_text.text = message
            info_bar.visible = true
            info_bar_timer.start()
        }
    }

    header: CustomToolBar {
        id: header_bar

        enableBack: true
        onBackClicked: backToShopEditor()

        enableSave: true
        isSaved: shop_tool && shop_tool.editor
                 && shop_tool.editor.itemEditor ? shop_tool.editor.itemEditor.isSaved : true
        onSaveClicked: shop_tool.editor.itemEditor.save()
    }

    ItemDetails {
        id: item_details
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 200
    }

    Item {
        id: right_item

        anchors.left: item_details.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 5

        Rectangle {
            id: item_header
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: Defines.TOOLBAR_HEIGHT
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
            anchors.topMargin: 5

            clip: true
            spacing: 0

            ScrollBar.vertical: ScrollBar {}

            model: shop_tool && shop_tool.editor
                   && shop_tool.editor.itemEditor ? shop_tool.editor.itemEditor.itemModel : []

            delegate: Rectangle {
                height: delegate_row.height
                anchors.left: parent ? parent.left : undefined
                anchors.right: parent ? parent.right : undefined
                color: "transparent"

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
                        text: name
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 5
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Label {
                        text: price
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        clip: true
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Label {
                        text: category
                        width: (parent.width - parent.leftPadding * 2 - parent.spacing * 2) / 6
                        clip: true
                        elide: Text.ElideRight
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }

                    Label {
                        text: description
                        width: (parent.width - x - delete_button.width
                                - parent.spacing - parent.rightPadding)
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

                        onClicked: shop_tool.editor.itemEditor.deleteItem(index)
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
        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase
        visible: false

        Timer {
            id: info_bar_timer
            interval: 2000

            onTriggered: {
                info_bar.visible = false
                stop()
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
