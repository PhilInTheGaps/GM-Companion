import QtQuick 2.9
import QtQuick.Controls 2.2

import FontAwesome 2.0

Dialog {
    id: unsplash_dialog
    title: qsTr("Find icon from unsplash.com")

    modal: true

    standardButtons: Dialog.Ok | Dialog.Cancel

    property string imageSource

    contentItem: Row {
        spacing: 10

        ScrollView {
            width: 850
            height: 500

            contentWidth: -1
            clip: true

            Flow {
                anchors.left: parent.left
                anchors.right: parent.right

                spacing: 10

                Repeater {
                    model: unsplashImageListModel

                    Rectangle {
                        color: "grey"
                        width: (parent.width - parent.spacing * 3) / 4
                        height: width

                        Image {
                            id: image
                            source: "https://source.unsplash.com/" + modelData.id + "/500x500"
                            sourceSize.width: width
                            sourceSize.height: height
                            anchors.fill: parent

                            visible: status == Image.Ready
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
            }
        }

        Item {
            width: 150

            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Column {
                anchors.fill: parent
                spacing: 10

                TextField {
                    id: search_field
                    anchors.left: parent.left
                    anchors.right: parent.right
                    selectByMouse: true

                    onTextChanged: audio_editor.findUnsplashImages(
                                       search_field.text)
                }

                Text {
                    id: unsplash_author
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Text {
                    id: unsplash_author_id
                    anchors.left: parent.left
                    anchors.right: parent.right
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Item {
                    height: unsplash_link_row.height
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Row {
                        id: unsplash_link_row
                        visible: unsplash_author_id.text != ""
                        spacing: 5

                        Text {
                            text: FontAwesome.externalLinkAlt
                            font.family: FontAwesome.familySolid
                        }

                        Text {
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

            Button {
                text: qsTr("Shuffle")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                onClicked: audio_editor.shuffleUnsplashImages()
            }
        }
    }
}
