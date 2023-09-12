import QtQuick
import QtQuick.Controls

// Zoom functionality adapted from an example by oniongarlic:
// https://github.com/oniongarlic/qtquick-flickable-image-zoom
Item {
    id: root

    property bool fitToScreenActive: true
    property real minZoom: 0.1
    property real maxZoom: 5
    property real zoomStep: 0.25

    property alias image: image_element

    readonly property bool isRotated: image_element.rotation === 90
                                      || image_element.rotation === 270

    function scaleImage(value) {
        fitToScreenActive = false

        if (value < minZoom) {
            image_element.scale = minZoom
        } else {
            image_element.scale = value
        }
    }

    function fitToScreen() {
        var w = flickable.width / (isRotated ? image_element.height : image_element.width)
        var h = flickable.height / (isRotated ? image_element.width : image_element.height)
        var s = Math.min(w, h, 1)

        image_element.scale = s
        minZoom = s
        image_element.prevScale = scale
        fitToScreenActive = true
        flickable.returnToBounds()
    }

    function zoomIn() {
        if (image_element.scale < maxZoom) {
            image_element.scale += zoomStep
        }

        flickable.returnToBounds()
        fitToScreenActive = false
        flickable.returnToBounds()
    }

    function zoomOut() {
        if (image_element.scale > minZoom) {
            image_element.scale -= zoomStep
        } else {
            image_element.scale = minZoom
        }

        flickable.returnToBounds()
        fitToScreenActive = false
        flickable.returnToBounds()
    }

    function zoomFull() {
        image_element.scale = 1
        fitToScreenActive = false
        flickable.returnToBounds()
    }

    function rotateLeft() {
        image_element.rotation -= 90

        if (image_element.rotation < 0) {
            image_element.rotation = 270
        }

        if (fitToScreenActive) {
            fitToScreen()
        }
    }

    function rotateRight() {
        image_element.rotation += 90

        if (image_element.rotation > 270) {
            image_element.rotation = 0
        }

        if (fitToScreenActive) {
            fitToScreen()
        }
    }

    Shortcut {
        sequences: [StandardKey.ZoomIn]
        onActivated: root.zoomIn()
        context: Qt.ApplicationShortcut
    }

    Shortcut {
        sequences: [StandardKey.ZoomOut]
        onActivated: root.zoomOut()
        context: Qt.ApplicationShortcut
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
            if (root.fitToScreenActive)
                root.fitToScreen()
        }

        onHeightChanged: {
            if (root.fitToScreenActive)
                root.fitToScreen()
        }

        Item {
            id: image_container
            width: Math.max(
                       (root.isRotated ? image_element.height : image_element.width) * image_element.scale,
                       flickable.width)
            height: Math.max(
                        (root.isRotated ? image_element.width : image_element.height) * image_element.scale,
                        flickable.height)

            Image {
                id: image_element

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
                        root.fitToScreen()
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: pressed
                                 || flickable.dragging ? Qt.ClosedHandCursor : Qt.OpenHandCursor

                    onWheel: function (wheel) {
                        if (wheel.modifiers & Qt.ControlModifier) {
                            if (wheel.angleDelta.y > 0)
                                root.zoomIn()
                            else
                                root.zoomOut()
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
        visible: image_element.source !== "" && image_element.status === Image.Loading
    }

    PinchArea {
        id: pinch_area
        anchors.fill: flickable
        enabled: image_element.status === Image.Ready
        pinch.target: image_element
        pinch.maximumScale: root.maxZoom
        pinch.minimumScale: root.minZoom

        onPinchStarted: {
            flickable.interactive = false
        }

        onPinchUpdated: pinch => {
                            flickable.contentX += pinch.previousCenter.x - pinch.center.x
                            flickable.contentY += pinch.previousCenter.y - pinch.center.y
                        }

        onPinchFinished: {
            flickable.interactive = true
            flickable.returnToBounds()
        }
    }
}
