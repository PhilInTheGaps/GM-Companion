import QtQuick
import QtQuick.Controls
import IconFonts
import src

Dialog {
    id: root

    property bool hasMessages: repeater.count

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
            onClicked: MessageManager.clearMessages()
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
                id: repeater
                model: MessageManager.messages

                Rectangle {
                    id: message_box

                    required property date timestamp
                    required property int type
                    required property string category
                    required property string body

                    color: "transparent"
                    border.color: switch (type) {
                                  case 1:
                                      "orange"
                                      break
                                  case 2:
                                  case 3:
                                      "red"
                                      break
                                  default:
                                      palette.button
                                  }

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

                        height: 25

                        color: palette.dark

                        Row {
                            anchors.fill: parent
                            anchors.margins: 5
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            spacing: 10

                            Label {
                                id: icon_label
                                font.family: FontAwesome.fontSolid.family
                                font.styleName: FontAwesome.fontSolid.styleName
                                font.pointSize: 12
                                verticalAlignment: Text.AlignVCenter
                                text: message_box.type === 4 ? FontAwesome.circleInfo : FontAwesome.triangleExclamation

                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                            }

                            Label {
                                id: timestamp_label

                                anchors.top: parent.top
                                anchors.bottom: parent.bottom

                                text: message_box.timestamp.toLocaleString(
                                          Qt.locale("en_US"),
                                          Locale.ShortFormat)
                                font.pointSize: 10
                                wrapMode: Label.NoWrap
                                verticalAlignment: Text.AlignVCenter
                            }

                            Label {
                                id: category_label
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom

                                text: message_box.category
                                font.pointSize: 10
                                wrapMode: Label.NoWrap
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }

                    TextField {
                        id: text_field
                        anchors.top: header.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        padding: 10

                        text: message_box.body
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
