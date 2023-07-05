import QtQuick 2.9
import QtQuick.Controls 2.2

// Zoom functionality adapted from an example by oniongarlic:
// https://github.com/oniongarlic/qtquick-flickable-image-zoom
Item {
    id: root

    property bool fitToScreenActive: true
    property real minZoom: 0.1
    property real maxZoom: 5
    property real zoomStep: 0.25

    property alias image: image

    readonly property bool isRotated: image.rotation == 90
                                      || image.rotation == 270

    function scaleImage(value) {
        fitToScreenActive = false

        if (value < minZoom) {
            image.scale = minZoom
        } else {
            image.scale = value
        }
    }

    function fitToScreen() {
        var w = flickable.width / (isRotated ? image.height : image.width)
        var h = flickable.height / (isRotated ? image.width : image.height)
        var s = Math.min(w, h, 1)

        image.scale = s
        minZoom = s
        image.prevScale = scale
        fitToScreenActive = true
        flickable.returnToBounds()
    }

    function zoomIn() {
        if (image.scale < maxZoom) {
            image.scale += zoomStep
        }

        flickable.returnToBounds()
        fitToScreenActive = false
        flickable.returnToBounds()
    }

    function zoomOut() {
        if (image.scale > minZoom) {
            image.scale -= zoomStep
        } else {
            image.scale = minZoom
        }

        flickable.returnToBounds()
        fitToScreenActive = false
        flickable.returnToBounds()
    }

    function zoomFull() {
        image.scale = 1
        fitToScreenActive = false
        flickable.returnToBounds()
    }

    function rotateLeft() {
        image.rotation -= 90

        if (image.rotation < 0) {
            image.rotation = 270
        }

        if (fitToScreenActive) {
            fitToScreen()
        }
    }

    function rotateRight() {
        image.rotation += 90

        if (image.rotation > 270) {
            image.rotation = 0
        }

        if (fitToScreenActive) {
            fitToScreen()
        }
    }

    Connections {
        target: main_window

        function onZoomIn() {
            zoomIn()
        }

        function onZoomOut() {
            zoomOut()
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent

        boundsBehavior: Flickable.DragOverBounds
        contentHeight: image_container.height
        contentWidth: image_container.width
        clip: true
        interactive: true

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
            width: Math.max(
                       (isRotated ? image.height : image.width) * image.scale,
                       flickable.width)
            height: Math.max(
                        (isRotated ? image.width : image.height) * image.scale,
                        flickable.height)

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
                        fitToScreen()
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: pressed
                                 || flickable.dragging ? Qt.ClosedHandCursor : Qt.OpenHandCursor

                    onWheel: function (wheel) {
                        if (wheel.modifiers & Qt.ControlModifier) {
                            if (wheel.angleDelta.y > 0)
                                zoomIn()
                            else
                                zoomOut()
                        }
                    }
                }
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {}
        ScrollIndicator.horizontal: ScrollIndicator {}
    }

    // Busy Indicator
    BusyIndicator {
        anchors.centerIn: parent
        visible: image.source !== "" && image.status === Image.Loading
    }

    PinchArea {
        id: pinch_area
        anchors.fill: flickable
        enabled: image.status === Image.Ready
        pinch.target: image
        pinch.maximumScale: maxZoom
        pinch.minimumScale: minZoom

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
}
