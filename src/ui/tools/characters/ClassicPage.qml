import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../components"
import FontAwesome 2.0

Item {
    id: root

    // Right control bar
    Rectangle {
        id: control_bar
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: color_scheme.toolbarHeight
        color: color_scheme.menuColor

        Column {
            anchors.fill: parent
            spacing: 10
            topPadding: 5

            // Larger
            ToolBarIconButton {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: undefined
                anchors.bottom: undefined
                height: width

                fa_icon: FontAwesome.plus

                onClicked: {

                    character_image.height *= 2
                    character_image.width *= 2

                    character_flickable.contentY = character_flickable.contentHeight
                            / 2 - character_flickable.height / 2
                    character_flickable.contentX = character_flickable.contentWidth
                            / 2 - character_flickable.width / 2
                }
            }

            // Smaller
            ToolBarIconButton {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: undefined
                anchors.bottom: undefined
                height: width

                fa_icon: FontAwesome.minus

                onClicked: {
                    character_image.height *= 0.5
                    character_image.width *= 0.5

                    character_flickable.contentY = character_flickable.contentHeight
                            / 2 - character_flickable.height / 2
                    character_flickable.contentX = character_flickable.contentWidth
                            / 2 - character_flickable.width / 2
                }
            }

            // Fit
            ToolBarIconButton {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: undefined
                anchors.bottom: undefined
                height: width

                fa_icon: FontAwesome.expand

                onClicked: {
                    character_image.width = character_flickable.width
                    character_image.height = character_flickable.height
                }
            }

            // Rotate Left
            ToolBarIconButton {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: undefined
                anchors.bottom: undefined
                height: width

                fa_icon: FontAwesome.undo

                onClicked: {
                    character_image.rotation -= 90
                }
            }

            // Rotate Right
            ToolBarIconButton {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: undefined
                anchors.bottom: undefined
                height: width

                fa_icon: FontAwesome.redo

                onClicked: {
                    character_image.rotation += 90
                }
            }
        }
    }

    // Main Flickable
    Flickable {
        id: character_flickable
        anchors.left: parent.left
        anchors.right: control_bar.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        clip: true
        interactive: true

        contentWidth: character_image.width
        contentHeight: character_image.height
        boundsBehavior: Flickable.StopAtBounds

        PinchArea {
            width: Math.max(character_flickable.contentWidth,
                            character_flickable.width)
            height: Math.max(character_flickable.contentHeight,
                             character_flickable.height)

            property real initialWidth
            property real initialHeight

            onPinchStarted: {
                initialWidth = character_flickable.contentWidth
                initialHeight = character_flickable.contentHeight
            }

            onPinchUpdated: {
                // adjust content pos due to drag
                character_flickable.contentX += pinch.previousCenter.x - pinch.center.x
                character_flickable.contentY += pinch.previousCenter.y - pinch.center.y

                // resize content
                character_flickable.resizeContent(initialWidth * pinch.scale,
                                                  initialHeight * pinch.scale,
                                                  pinch.center)

                character_image.width = character_flickable.contentWidth
                character_image.height = character_flickable.contentHeight
            }

            onPinchFinished: {
                // Move its content within bounds.
                character_flickable.returnToBounds()
            }
        }

        Rectangle {
            width: character_image.width
            height: character_image.height
            color: color_scheme.backgroundColor

            Image {
                id: character_image
                width: character_flickable.width
                height: character_flickable.height
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                source: character_image_viewer.image
            }
        }
    }

    Item {
        anchors.left: character_flickable.left
        anchors.right: character_flickable.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: character_image_viewer.image != ""
                 && character_image.status !== Image.Ready

        BusyIndicator {
            anchors.centerIn: parent
        }
    }

    Item {
        id: image_overlay_item
        anchors.left: character_flickable.left
        anchors.right: character_flickable.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        visible: character_tool.categories.length > 1

        PageIndicator {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            count: character_tool.categories.length
            currentIndex: character_image_viewer.imageIndex

            delegate: Rectangle {
                implicitWidth: 8
                implicitHeight: 8

                radius: width / 2
                color: color_scheme.menuColor

                opacity: index === character_image_viewer.imageIndex ? 1 : 0.7
            }
        }

        ToolBarIconButton {
            anchors.top: undefined
            anchors.bottom: undefined
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            fa_icon: FontAwesome.chevronLeft
            icon_color: color_scheme.textColor
            height: color_scheme.toolbarHeight

            onClicked: character_image_viewer.nextImage(false)
        }

        ToolBarIconButton {
            anchors.top: undefined
            anchors.bottom: undefined
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            fa_icon: FontAwesome.chevronRight
            icon_color: color_scheme.textColor
            height: color_scheme.toolbarHeight

            onClicked: character_image_viewer.nextImage(true)
        }
    }
}
