import QtQuick
import QtQuick.Controls
import src
import "../../common"

Item {
    id: root

    required property Label markerNameLabel

    signal toMarkerList
    signal toMarkerDetails
    signal markerMenuToggled
    signal openDeleteDialog

    function setImageSource(source) {
        image_viewer.image.source = source;
        image_viewer.image.rotation = 0;
    }

    ImageViewer {
        id: image_viewer
        anchors.fill: parent

        image.source: MapTool.currentMap ? MapTool.currentMap.imageData : ""

        MapMarkerLayer {
            parent: image_viewer.image
            markerNameLabel: root.markerNameLabel
            onOpenDeleteDialog: root.openDeleteDialog()
        }
    }

    MapViewerControlBar {
        id: control_bar
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

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

        onToggleMarkerMenu: {
            root.markerMenuToggled();
        }
    }
}
