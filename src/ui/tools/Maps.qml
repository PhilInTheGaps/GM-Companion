import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import "maps"
import FontAwesome 2.0

// Zoom functionality adapted from an example by oniongarlic:
// https://github.com/oniongarlic/qtquick-flickable-image-zoom
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

        onCurrentIndexChanged: {
            map_tool.setCurrentCategory(currentIndex)
        }

        background: Rectangle {
            color: color_scheme.toolbarColor
        }

        Repeater {
            id: tab_button_repeater

            model: map_tool.categories

            onItemAdded: {
                if (index == 0) {
                    maps_tab_bar.currentIndex = 0
                }
            }

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

    Rectangle {
        id: left_item
        color: color_scheme.secondaryBackgroundColor
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 200

        ScrollView {
            id: left_item_scrollview

            anchors.fill: parent
            anchors.bottomMargin: 5
            anchors.topMargin: 5

            contentHeight: c.height
            contentWidth: -1
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
    }

    Flickable {
        id: flickable
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: left_item.visible ? left_item.right : parent.left
        anchors.right: maps_control_bar.left

        boundsBehavior: Flickable.StopAtBounds
        contentHeight: image_container.height
        contentWidth: image_container.width
        clip: true
        interactive: true

        property bool fitToScreenActive: true
        property real minZoom: 0.1
        property real maxZoom: 5
        property real zoomStep: 0.25

        onWidthChanged: {
            if (fitToScreenActive)
                fitToScreen()
        }
        onHeightChanged: {
            if (fitToScreenActive)
                fitToScreen()
        }

        Item {
            id: image_container
            width: Math.max(image.width * image.scale, flickable.width)
            height: Math.max(image.height * image.scale, flickable.height)

            Image {
                id: image

                property real prevScale: 1.0

                anchors.centerIn: parent
                transformOrigin: Item.Center

                asynchronous: true
                cache: false
                smooth: true
                fillMode: Image.PreserveAspectFit

                onScaleChanged: {
                    if ((width * scale) > flickable.width) {
                        var xoff = (flickable.width / 2 + flickable.contentX) * scale / prevScale
                        flickable.contentX = xoff - flickable.width / 2
                    }

                    if ((height * scale) > flickable.height) {
                        var yoff = (flickable.height / 2 + flickable.contentY) * scale / prevScale
                        flickable.contentY = yoff - flickable.height / 2
                    }

                    prevScale = scale
                }

                onStatusChanged: {
                    if (status === Image.Ready) {
                        flickable.fitToScreen()
                    }
                }
            }
        }

        function fitToScreen() {
            var s = Math.min(flickable.width / image.width,
                             flickable.height / image.height, 1)
            image.scale = s
            flickable.minZoom = s
            image.prevScale = scale
            fitToScreenActive = true
            flickable.returnToBounds()
        }

        function zoomIn() {
            if (image.scale < flickable.maxZoom)
                image.scale += zoomStep
            flickable.returnToBounds()
            fitToScreenActive = false
            flickable.returnToBounds()
        }

        function zoomOut() {
            if (image.scale > flickable.minZoom)
                image.scale -= zoomStep
            else
                image.scale = flickable.minZoom
            flickable.returnToBounds()
            fitToScreenActive = false
            flickable.returnToBounds()
        }

        function zoomFull() {
            image.scale = 1
            fitToScreenActive = false
            flickable.returnToBounds()
        }

        ScrollIndicator.vertical: ScrollIndicator {
        }

        ScrollIndicator.horizontal: ScrollIndicator {
        }
    }

    PinchArea {
        id: pinch_area
        anchors.fill: flickable
        enabled: image.status === Image.Ready
        pinch.target: image
        pinch.maximumScale: flickable.maxZoom
        pinch.minimumScale: flickable.minZoom

        onPinchStarted: {
            flickable.interactive = false
        }

        onPinchUpdated: {
            flickable.contentX += pinch.previousCenter.x - pinch.center.x
            flickable.contentY += pinch.previousCenter.y - pinch.center.y
        }

        onPinchFinished: {
            flickable.interactive = true
            flickable.returnToBounds()
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
                id: larger_button
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

                onClicked: flickable.zoomIn()
            }

            Slider {
                id: scale_slider
                orientation: Qt.Vertical
                anchors.horizontalCenter: parent.horizontalCenter

                from: 0 //flickable.minZoom
                to: flickable.maxZoom
                stepSize: flickable.zoomStep
                snapMode: Slider.SnapAlways
                value: image.scale

                onMoved: {
                    if (value < flickable.minZoom)
                        image.scale = flickable.minZoom
                    else
                        image.scale = value
                }

                background: Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    y: scale_slider.topPadding
                    implicitWidth: 6
                    implicitHeight: 200
                    width: implicitWidth
                    height: scale_slider.availableHeight
                    radius: 2
                    color: "#6c7a89" //"#21be2b"

                    Rectangle {
                        width: parent.width
                        height: scale_slider.visualPosition * parent.height
                        color: "#bdbebf"
                        radius: 2
                    }
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

                onClicked: flickable.zoomOut()
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

                onClicked: flickable.fitToScreen()
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
                    image.rotation -= 90
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
                    image.rotation += 90
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
