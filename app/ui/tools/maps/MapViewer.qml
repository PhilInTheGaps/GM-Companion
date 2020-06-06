import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../common"

Item {
    id: root

    signal toMarkerList
    signal toMarkerDetails
    signal markerMenuToggled

    function openDeleteDialog() {
        marker_delete_rect.visible = true
    }

    function setImageSource(source) {
        image_viewer.image.source = source
        image_viewer.image.rotation = 0
    }

    ImageViewer {
        id: image_viewer
        anchors.fill: parent

        image.source: map_tool.currentMap ? map_tool.currentMap.imageData : ""

        Component.onCompleted: {
            var component = Qt.createComponent("MapMarkerLayer.qml")
            if (component.status === Component.Ready) {
                component.createObject(image_viewer.image)
            }
        }
    }

    // Delete Marker ?
    Rectangle {
        id: marker_delete_rect
        anchors.horizontalCenter: image_viewer.horizontalCenter
        anchors.bottom: image_viewer.bottom
        anchors.bottomMargin: 20

        width: marker_delete_rect_row.width
        height: marker_delete_rect_row.height
        color: palette.dark
        visible: false

        Row {
            id: marker_delete_rect_row
            anchors.centerIn: parent
            spacing: 10
            padding: 10

            Label {
                text: qsTr("Delete?")
                font.pointSize: 12
                anchors.verticalCenter: parent.verticalCenter
            }

            CustomToolBarButton {
                iconText: FontAwesome.check
                anchors.top: undefined
                anchors.bottom: undefined
                height: 30

                onClicked: {
                    marker_delete_rect.visible = false
                    map_tool.deleteMarker(map_tool.markerIndex)
                    toMarkerList()
                }
            }

            CustomToolBarButton {
                iconText: FontAwesome.times
                anchors.top: undefined
                anchors.bottom: undefined
                height: 30

                onClicked: marker_delete_rect.visible = false
            }
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
        onSetScale: image_viewer.scaleImage(value)
        onRotateLeft: image_viewer.rotateLeft()
        onRotateRight: image_viewer.rotateRight()
        onFitToScreen: image_viewer.fitToScreen()

        onToggleMarkerMenu: {
            markerMenuToggled()
        }
    }
}
