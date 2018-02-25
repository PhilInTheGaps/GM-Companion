import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

Page {
    id: maps_page
    Row {
        anchors.fill: parent
        TabView {
            id: maps_tab_view
            height: parent.height
            width: 200
            Tab {
                title: "Test"

                ListView {
                    id: list1
                    Text {
                        text: "BLA"
                    }
                }
            }

            Tab {
                title: "Test 2"

                ListView {
                    id: list2
                    Text {
                        text: "Test ASD"
                    }
                }
            }
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


                //                source: "file:/home/phil/drives/Google-Drive-Linux/GM-Boys/Regelwerk und Co./Das Schwarze Auge (5)/Karten/Aventurien.jpg"
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
        }

        Button {
            text: "L"
            width: parent.width
            height: width

            onClicked: maps_tab_view.visible ? maps_tab_view.visible
                                               = false : maps_tab_view.visible = true
        }
    }
}
