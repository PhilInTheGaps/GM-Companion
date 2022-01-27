import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Dialog {
    id: root

    title: qsTr("Warnings and Errors")

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true

    Row {
        id: button_row
        spacing: 5

        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Button {
            text: qsTr("Clear")
            onClicked: message_manager.clearMessages()
        }

        Button {
            text: qsTr("Close")
            onClicked: root.close()
        }
    }

    Flickable {
        id: flickable
        anchors.top: parent.top
        anchors.bottom: button_row.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: 10

        contentHeight: message_column.implicitHeight
        contentWidth: -1
        interactive: true
        clip: true

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: flickable.contentHeight > flickable.height
        }

        Column {
            id: message_column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0
            spacing: 10

            Repeater {
                model: message_manager.messages

                Rectangle {
                    color: "transparent"
                    border.color: modelData.type
                                  === 1 ? "orange" : modelData.type === 2
                                          || modelData.type === 3 ? "red" : palette.button
                    border.width: 1
                    height: header.height + text_field.height
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Rectangle {
                        id: header
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.margins: parent.border.width

                        height: icon_label.height + 10

                        color: palette.dark

                        Label {
                            id: icon_label
                            font.family: FontAwesome.familySolid
                            font.pointSize: 12
                            verticalAlignment: Text.AlignVCenter
                            text: modelData.type
                                  === 4 ? FontAwesome.infoCircle : FontAwesome.exclamationTriangle

                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.margins: 5
                            anchors.leftMargin: 10
                        }

                        Label {
                            id: timestamp_label
                            anchors.top: parent.top
                            anchors.left: icon_label.right
                            anchors.right: parent.right
                            anchors.margins: 5
                            anchors.leftMargin: 10

                            text: modelData.timestamp.toLocaleString(
                                      Qt.locale("en_US"), Locale.ShortFormat)
                            font.pointSize: 10
                            wrapMode: Label.NoWrap
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    TextField {
                        id: text_field
                        anchors.top: header.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        padding: 10

                        text: modelData.body
                        font.pointSize: 12
                        wrapMode: Label.WrapAtWordBoundaryOrAnywhere
                        readOnly: true
                        selectByMouse: true
                        background: Item {}
                    }
                }
            }
        }
    }
}
