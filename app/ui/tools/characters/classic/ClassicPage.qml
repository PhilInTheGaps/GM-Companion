import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
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

        image.source: CharacterTool.imageViewer.image
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
        onSetScale: value => image_viewer.scaleImage(value)
        onRotateLeft: image_viewer.rotateLeft()
        onRotateRight: image_viewer.rotateRight()
        onFitToScreen: image_viewer.fitToScreen()
    }

    Item {
        id: image_overlay_item
        anchors.fill: image_viewer
        visible: CharacterTool.categories.length > 1

        PageIndicator {
            id: indicator
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            count: CharacterTool.categories.length
            currentIndex: CharacterTool.imageViewer.imageIndex

            delegate: Rectangle {
                required property int index

                implicitWidth: 8
                implicitHeight: 8

                radius: width / 2
                color: palette.alternateBase

                opacity: index === CharacterTool.imageViewer.imageIndex ? 1 : 0.7
            }
        }

        CustomButton {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            usesFixedWidth: false

            iconText: FontAwesome.chevronLeft

            onClicked: CharacterTool.imageViewer.nextImage(false)
        }

        CustomButton {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            usesFixedWidth: false

            iconText: FontAwesome.chevronRight

            onClicked: CharacterTool.imageViewer.nextImage(true)
        }
    }
}
