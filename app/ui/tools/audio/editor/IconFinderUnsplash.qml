import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import CustomComponents 1.0

Dialog {
    id: unsplash_dialog
    title: qsTr("Find icon from unsplash.com")

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    width: parent.width * 0.8
    height: parent.height * 0.8

    modal: true

    standardButtons: Dialog.Ok | Dialog.Cancel

    property string imageSource

    contentItem: Item {
        Item {
            id: header_item
            height: search_field.height
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            TextField {
                id: search_field
                anchors.left: parent.left
                anchors.right: shuffle_button.left
                selectByMouse: true
                placeholderText: qsTr("Type to search ...")

                onTextChanged: audio_editor.findUnsplashImages(
                                   search_field.text)
            }

            Button {
                id: shuffle_button
                text: qsTr("Shuffle")
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                onClicked: audio_editor.shuffleUnsplashImages()
            }
        }

        Item {
            id: main_item
            anchors.top: header_item.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: 5

            GridView {
                id: grid
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: detail_item.left
                anchors.bottom: parent.bottom
                anchors.rightMargin: 5

                clip: true
                model: unsplashImageListModel
                cellWidth: width / 5
                cellHeight: cellWidth

                ScrollBar.vertical: ScrollBar {
                    id: scroll_bar
                }

                delegate: Rectangle {
                    width: grid.cellWidth - 10
                    height: grid.cellHeight - 10
                    color: "grey"

                    Image {
                        id: image
                        source: "https://source.unsplash.com/" + modelData.id + "/500x500"
                        sourceSize.width: width
                        sourceSize.height: height
                        anchors.fill: parent

                        visible: status == Image.Ready
                    }

                    BusyIndicator {
                        id: busy
                        anchors.centerIn: parent
                        visible: !image.visible
                    }

                    Rectangle {
                        anchors.fill: parent
                        border.color: "black"
                        border.width: 2
                        color: "transparent"
                        visible: image_mouse_area.containsMouse
                    }

                    MouseArea {
                        id: image_mouse_area
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            unsplash_author.text = modelData.author
                            unsplash_author_id.text = "@" + modelData.authorId
                            imageSource = image.source
                        }
                    }
                }
            }

            Column {
                id: detail_item
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 150
                spacing: 10

                Label {
                    id: unsplash_author
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Label {
                    id: unsplash_author_id
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Label {
                    height: unsplash_link_row.height
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Row {
                        id: unsplash_link_row
                        visible: unsplash_author_id.text != ""
                        spacing: 5

                        Label {
                            text: FontAwesome.externalLinkAlt
                            font.family: FontAwesome.familySolid
                        }

                        Label {
                            text: "Unsplash"
                        }
                    }

                    MouseArea {
                        anchors.fill: unsplash_link_row
                        onClicked: Qt.openUrlExternally(
                                       "https://unsplash.com/" + unsplash_author_id.text)
                    }
                }
            }
        }
    }
}
