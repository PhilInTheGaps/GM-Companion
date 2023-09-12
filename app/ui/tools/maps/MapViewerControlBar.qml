import QtQuick
import QtQuick.Controls
import IconFonts
import CustomComponents
import "../.."

Column {
    id: root

    property double minZoom: 0
    property double maxZoom: 2
    property double zoomStep: 0.5
    property double currentScale: 1

    signal zoomIn
    signal zoomOut
    signal setScale(double value)
    signal rotateLeft
    signal rotateRight
    signal fitToScreen
    signal toggleMarkerMenu

    width: Sizes.toolbarWidth
    spacing: 5
    topPadding: 5

    CustomToolBarButton {
        id: fit_button
        iconText: FontAwesome.expand
        onClicked: root.fitToScreen()
        verticalMode: true
        outline: true
        height: width
    }

    CustomToolBarButton {
        id: larger_button
        iconText: FontAwesome.plus
        onClicked: root.zoomIn()
        verticalMode: true
        outline: true
        height: width
    }

    Slider {
        id: scale_slider
        orientation: Qt.Vertical
        anchors.horizontalCenter: parent.horizontalCenter

        from: root.minZoom
        to: root.maxZoom
        stepSize: root.zoomStep
        snapMode: Slider.SnapAlways
        value: root.currentScale

        onMoved: root.setScale(value)

        handle: Rectangle {
            x: scale_slider.leftPadding + scale_slider.availableWidth / 2 - width / 2
            y: scale_slider.bottomPadding + scale_slider.visualPosition
               * (scale_slider.availableHeight - height)
            implicitWidth: 26
            implicitHeight: 26
            radius: 13
            color: scale_slider.pressed ? palette.light : palette.midlight
            border.color: palette.dark
        }

        background: Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            y: scale_slider.topPadding
            implicitWidth: 6
            implicitHeight: 200
            width: implicitWidth
            height: scale_slider.availableHeight
            radius: 2
            color: palette.button
            border.color: palette.dark

            Rectangle {
                width: parent.width
                height: scale_slider.visualPosition * parent.height
                color: palette.midlight
                radius: 2
                border.color: palette.dark
            }
        }
    }

    CustomToolBarButton {
        id: smaller_button
        iconText: FontAwesome.minus
        onClicked: root.zoomOut()
        verticalMode: true
        outline: true
        height: width
    }

    CustomToolBarButton {
        id: rotate_left_button
        iconText: FontAwesome.rotateLeft
        onClicked: root.rotateLeft()
        verticalMode: true
        outline: true
        height: width
    }

    CustomToolBarButton {
        id: rotate_right_button
        iconText: FontAwesome.rotateRight
        onClicked: root.rotateRight()
        verticalMode: true
        outline: true
        height: width
    }

    CustomToolBarButton {
        id: marker_button
        iconText: FontAwesome.locationDot
        iconColor: "red"
        onClicked: root.toggleMarkerMenu()
        verticalMode: true
        outline: true
        height: width
    }
}
