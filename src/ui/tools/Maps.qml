import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import "maps"
import FontAwesome 2.0

Page {
    id: maps_page

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: maps_tab_bar

    TabBar {
        id: maps_tab_bar

        anchors.left: parent.left
        anchors.right: parent.right

        height: platform.isAndroid ? width / 6 : color_scheme.toolbarHeight
        currentIndex: 0

        onCurrentIndexChanged: {
            map_tool.setCurrentCategory(currentIndex)
        }

        background: Rectangle {
            color: color_scheme.toolbarColor
        }

        Repeater {
            id: tab_button_repeater

            model: map_tool.categories

            TabButton {
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: modelData
                    color: color_scheme.toolbarTextColor
                    font.pointSize: 12
                    font.bold: true
                    anchors.centerIn: parent
                }

                Rectangle {
                    color: "white"
                    height: 2
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: 5
                    visible: maps_tab_bar.currentIndex == index
                }

                background: Rectangle {
                    color: color_scheme.toolbarColor
                }

                onClicked: {
                    left_item.visible = true
                }
            }
        }
    }

    ScrollView {
        id: left_item
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        width: 200

        contentHeight: c.height
        clip: true

        Column {
            id: c
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
            spacing: 10

            Repeater {
                model: mapListModel

                MapListItem {
                    name: modelData.name
                    path: modelData.path
                }
            }
        }
    }

    Flickable {
        id: maps_image_flickable
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: left_item.visible ? left_item.right : parent.left
        anchors.right: maps_control_bar.left

        clip: true
        interactive: true

        contentWidth: maps_image.width
        contentHeight: maps_image.height

        // For touchscreen zooming
        PinchArea {
            width: Math.max(maps_image_flickable.contentWidth,
                            maps_image_flickable.width)
            height: Math.max(maps_image_flickable.contentHeight,
                             maps_image_flickable.height)

            property real initialWidth
            property real initialHeight

            onPinchStarted: {
                initialWidth = maps_image_flickable.contentWidth
                initialHeight = maps_image_flickable.contentHeight
            }

            onPinchUpdated: {
                // adjust content pos due to drag
                maps_image_flickable.contentX += pinch.previousCenter.x - pinch.center.x
                maps_image_flickable.contentY += pinch.previousCenter.y - pinch.center.y

                // resize content
                maps_image_flickable.resizeContent(initialWidth * pinch.scale,
                                                   initialHeight * pinch.scale,
                                                   pinch.center)

                maps_image.width = maps_image_flickable.contentWidth
                maps_image.height = maps_image_flickable.contentHeight
            }

            onPinchFinished: {
                // Move its content within bounds.
                maps_image_flickable.returnToBounds()
            }
        }

        Rectangle {
            width: maps_image.width
            height: maps_image.height
            color: color_scheme.backgroundColor

            Image {
                id: maps_image
                height: maps_image_flickable.height
                width: maps_image_flickable.width
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    Rectangle {
        id: maps_control_bar
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: color_scheme.toolbarHeight
        color: color_scheme.menuColor

        Column {
            anchors.fill: parent

            // Larger
            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                Text {
                    text: FontAwesome.plus
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    maps_image.height *= 2
                    maps_image.width *= 2

                    maps_image_flickable.contentY = maps_image_flickable.contentHeight
                            / 2 - maps_image_flickable.height / 2
                    maps_image_flickable.contentX = maps_image_flickable.contentWidth
                            / 2 - maps_image_flickable.width / 2
                }
            }

            // Smaller
            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                Text {
                    text: FontAwesome.minus
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    maps_image.height *= 0.5
                    maps_image.width *= 0.5

                    maps_image_flickable.contentY = maps_image_flickable.contentHeight
                            / 2 - maps_image_flickable.height / 2
                    maps_image_flickable.contentX = maps_image_flickable.contentWidth
                            / 2 - maps_image_flickable.width / 2
                }
            }

            // Fit
            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                Text {
                    text: FontAwesome.expand
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    maps_image.height = maps_image_flickable.height
                    maps_image.width = maps_image_flickable.width
                }
            }

            // Rotate Left
            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                Text {
                    text: FontAwesome.undo
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    maps_image.rotation -= 90
                }
            }

            // Rotate Right
            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                Text {
                    text: FontAwesome.redo
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: {
                    maps_image.rotation += 90
                }
            }

            // Show / Hide left Item
            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                Text {
                    text: FontAwesome.bars
                    font.family: FontAwesome.familySolid
                    font.pixelSize: height
                    anchors.fill: parent
                    anchors.margins: 10
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                background: Rectangle {
                    color: "transparent"
                }

                onClicked: left_item.visible = !left_item.visible
            }
        }
    }
}
