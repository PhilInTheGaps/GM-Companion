import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import gm.companion.maptool 1.0
import "maps"
import "../fontawesome"
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0

Page {
    id: maps_page

    PlatformDetails {
        id: platform
    }

    MapTool {
        id: map_tool

        Component.onCompleted: {
            findMaps()
        }

        function loadMap(path) {
            maps_image.source = "file:///" + path
        }

        onCategoriesChanged: {
            var mapCategories = categories

            for (var i = 0; i < mapCategories.length; i++) {
                var list = maps(mapCategories[i])

                var component = Qt.createComponent("./maps/MapListTab.qml")
                var tab = component.createObject(maps_swipe_view, {
                                                     "list": maps(mapCategories[i]),
                                                     "paths": mapPaths(
                                                                  mapCategories[i]),
                                                     "category": mapCategories[i]
                                                 })

                tab.clicked.connect(loadMap)

                tab_button_repeater.model++
            }

            maps_tab_bar.setCurrentIndex(0)
        }
    }

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Column {
        anchors.fill: parent

        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height - maps_control_bar.height

            Column {
                id: maps_tab_column
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: platform.isAndroid ? parent.width / 4 : 200

                TabBar {
                    id: maps_tab_bar
                    width: parent.width
                    height: platform.isAndroid ? width / 6 : color_scheme.toolbarHeight

                    currentIndex: maps_swipe_view.currentIndex

                    onCurrentIndexChanged: {
                        maps_swipe_view.currentIndex = currentIndex
                    }

                    Repeater {
                        id: tab_button_repeater

                        model: 0

                        TabButton {
                            text: map_tool.categories[index]
                            height: color_scheme.toolbarHeight
                            font.pointSize: 12
                        }
                    }
                }

                SwipeView {
                    id: maps_swipe_view
                    width: parent.width
                    height: parent.height - parent.spacing - maps_tab_bar.height

                    currentIndex: maps_tab_bar.currentIndex
                    clip: true
                }
            }

            Flickable {
                id: maps_image_flickable
                width: maps_tab_column.visible ? maps_page.width
                                                 - maps_tab_column.width : maps_page.width
                height: maps_page.height - maps_control_bar.height
                clip: true
                interactive: true

                contentWidth: maps_image.width
                contentHeight: maps_image.height

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
                        maps_image_flickable.resizeContent(
                                    initialWidth * pinch.scale,
                                    initialHeight * pinch.scale, pinch.center)

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
                        height: maps_page.height
                        width: maps_page.width - maps_tab_column.width
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }

        Rectangle {
            id: maps_control_bar
            height: platform.isAndroid ? parent.height / 12 : color_scheme.toolbarHeight
            width: parent.width
            color: color_scheme.toolbarColor

            Button {
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    icon: icons.fas_bars
                    pointSize: 25
                    anchors.centerIn: parent
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                }

                width: parent.height - 10
                height: width
                x: 5
                y: 5

                onClicked: maps_tab_column.visible ? maps_tab_column.visible
                                                     = false : maps_tab_column.visible = true
            }

            Row {
                id: controls
                height: parent.height
                padding: 5
                spacing: 5

                anchors.horizontalCenter: parent.horizontalCenter

                // Larger
                Button {
                    hoverEnabled: true

                    background: Rectangle {
                        color: "transparent"
                    }

                    Icon {
                        icon: icons.fas_plus
                        pointSize: 25
                        anchors.centerIn: parent
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    }

                    width: parent.height - parent.padding * 2
                    height: width

                    font.pointSize: height / 2

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
                    hoverEnabled: true

                    background: Rectangle {
                        color: "transparent"
                    }

                    Icon {
                        icon: icons.fas_minus
                        pointSize: 25
                        anchors.centerIn: parent
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    }

                    width: parent.height - parent.padding * 2
                    height: width

                    onClicked: {

                        maps_image.height *= 0.5
                        maps_image.width *= 0.5

                        maps_image_flickable.contentY = maps_image_flickable.contentHeight
                                / 2 - maps_image_flickable.height / 2
                        maps_image_flickable.contentX = maps_image_flickable.contentWidth
                                / 2 - maps_image_flickable.width / 2
                    }
                }

                // Reset
                Button {
                    hoverEnabled: true

                    background: Rectangle {
                        color: "transparent"
                    }

                    Icon {
                        icon: icons.fas_undo
                        pointSize: 25
                        anchors.centerIn: parent
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    }

                    width: parent.height - parent.padding * 2
                    height: width

                    onClicked: {
                        maps_image.height = maps_page.height
                        maps_image.width = maps_tab_column.visible ? maps_page.width - maps_tab_column.width : maps_page.width
                    }
                }
            }
        }
    }
}
