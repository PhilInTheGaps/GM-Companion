import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.maptool 1.0
import "maps"

import gm.companion.colorscheme 1.0

Page {
    id: maps_page

    MapTool {
        id: map_tool

        Component.onCompleted: {
            loadMapList()
        }

        function loadMap(path) {
            maps_image.source = "file:///" + path
        }

        function loadMapList() {
            findMaps()

            var mapCategories = categories

            for (var i = 0; i < mapCategories.length; i++) {
                var list = maps(mapCategories[i])

                var component = Qt.createComponent("./maps/MapListTab.qml")
                var tab = component.createObject(maps_swipe_view, {
                                                     list: maps(mapCategories[i]),
                                                     paths: mapPaths(
                                                                mapCategories[i]),
                                                     category: mapCategories[i]
                                                 })

                tab.clicked.connect(loadMap)

                tab_button_repeater.model++
            }
        }
    }

    ColorScheme {
        id: color_scheme
    }

    Column {
        anchors.fill: parent

        Row {
            width: parent.width
            height: parent.height - maps_control_bar.height

            Column {
                id: maps_tab_column
                height: parent.height
                width: 200

                TabBar {
                    id: maps_tab_bar
                    width: parent.width

                    Repeater {
                        id: tab_button_repeater

                        model: 0

                        TabButton {
                            text: map_tool.categories[index]
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
                height: maps_page.height
                clip: true
                interactive: true

                contentWidth: maps_image.width
                contentHeight: maps_image.height

                Image {
                    id: maps_image
                    height: maps_page.height
                    width: maps_page.width - maps_tab_column.width
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        Rectangle {
            id: maps_control_bar
            height: parent.height / 18
            width: parent.width
            color: color_scheme.menuColor

            Button {
                Image {
                    source: "/icons/menu/three_bars_dark.png"
                    width: parent.height / 1.5
                    height: width
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
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

                Button {
                    Image {
                        source: "/icons/menu/plus_sign_dark.png"
                        width: parent.height / 1.5
                        height: width
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
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

                Button {
                    Image {
                        source: "/icons/menu/vertical_bar_dark.png"
                        width: parent.height / 1.5
                        height: width
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
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

                Button {
                    Image {
                        source: "/icons/menu/ring_arrow_dark.png"
                        width: parent.height / 1.5
                        height: width

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        mirror: true
                    }

                    width: parent.height - parent.padding * 2
                    height: width

                    onClicked: {

                        maps_image.height = maps_page.height
                        maps_image.width = maps_page.width - maps_tab_column.width
                    }
                }
            }
        }
    }
}
