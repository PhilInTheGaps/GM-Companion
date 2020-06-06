import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../../defines.js" as Defines
import "../../../common"

Item {
    id: root

    ImageViewer {
        id: image_viewer
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: control_bar.left
        anchors.rightMargin: 5

        image.source: character_image_viewer.image
    }

    ImageControlBar {
        id: control_bar
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        minZoom: image_viewer.minZoom
        maxZoom: image_viewer.maxZoom
        zoomStep: image_viewer.zoomStep
        currentScale: image_viewer.image.scale

        onZoomIn: image_viewer.zoomIn()
        onZoomOut: image_viewer.zoomOut()
        onSetScale: image_viewer.scaleImage(value)
        onRotateLeft: image_viewer.rotateLeft()
        onRotateRight: image_viewer.rotateRight()
        onFitToScreen: image_viewer.fitToScreen()
    }

    Item {
        id: image_overlay_item
        anchors.fill: image_viewer
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
                color: palette.alternateBase

                opacity: index === character_image_viewer.imageIndex ? 1 : 0.7
            }
        }

        CustomButton {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            usesFixedWidth: false

            iconText: FontAwesome.chevronLeft

            onClicked: character_image_viewer.nextImage(false)
        }

        CustomButton {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            usesFixedWidth: false

            iconText: FontAwesome.chevronRight

            onClicked: character_image_viewer.nextImage(true)
        }
    }
}
