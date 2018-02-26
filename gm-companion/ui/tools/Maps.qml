import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import gm.companion.maptool 1.0
import "maps"

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
                var tab = component.createObject(maps_tab_view, {
                                                     list: maps(mapCategories[i]),
                                                     paths: mapPaths(
                                                                mapCategories[i]),
                                                     category: mapCategories[i]
                                                 })

                tab.clicked.connect(loadMap)
            }
        }
    }

    Row {
        anchors.fill: parent
        TabView {
            id: maps_tab_view
            height: parent.height
            width: 200
        }

        ScrollView {
            id: maps_image_scroll_view
            width: maps_tab_view.visible ? maps_page.width - maps_tab_view.width : maps_page.width
            height: maps_page.height

            flickableItem.interactive: true

            Image {
                id: maps_image
                height: maps_page.height
                width: maps_page.width - maps_tab_view.width
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    Column {
        width: 40
        padding: 5
        spacing: 5

        x: maps_tab_view.visible ? maps_tab_view.width + 10 : 10
        y: 10

        Button {
            text: "+"
            width: parent.width
            height: width

            onClicked: {
                maps_image.height *= 2
                maps_image.width *= 2

                maps_image_scroll_view.flickableItem.contentY
                        = maps_image_scroll_view.flickableItem.contentHeight / 2
                        - maps_image_scroll_view.height / 2
                maps_image_scroll_view.flickableItem.contentX
                        = maps_image_scroll_view.flickableItem.contentWidth / 2
                        - maps_image_scroll_view.width / 2
            }
        }

        Button {
            text: "-"
            width: parent.width
            height: width

            onClicked: {
                maps_image.height *= 0.5
                maps_image.width *= 0.5

                maps_image_scroll_view.flickableItem.contentY
                        = maps_image_scroll_view.flickableItem.contentHeight / 2
                        - maps_image_scroll_view.height / 2
                maps_image_scroll_view.flickableItem.contentX
                        = maps_image_scroll_view.flickableItem.contentWidth / 2
                        - maps_image_scroll_view.width / 2
            }
        }

        Button {
            text: "R"
            width: parent.width
            height: width

            onClicked: {
                maps_image.height = maps_page.height
                maps_image.width = maps_page.width - maps_tab_view.width
            }
        }

        Button {
            //            text: "L"
            Image {
                source: "/icons/menu/three_bars_dark.png"
                width: parent.width - 20
                height: width
                sourceSize.width: width
                sourceSize.height: height
                x: 10
                y: 10
            }

            width: parent.width
            height: width

            onClicked: maps_tab_view.visible ? maps_tab_view.visible
                                               = false : maps_tab_view.visible = true
        }
    }
}
