import QtQuick 2.9
import QtQuick.Controls 2.2

import "maps"
import "../components"
import FontAwesome 2.0

// Zoom functionality adapted from an example by oniongarlic:
// https://github.com/oniongarlic/qtquick-flickable-image-zoom
Page {
    id: maps_page

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    header: maps_tab_bar

    MarkerEditor {
        id: marker_editor
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
    }

    // Choose Category
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
                    left_swipe_view.currentIndex = 0
                    left_item.visible = true
                }
            }
        }
    }

    // Left Pane
    Rectangle {
        id: left_item
        color: color_scheme.secondaryBackgroundColor
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 225

        SwipeView {
            id: left_swipe_view
            anchors.fill: parent
            clip: true
            interactive: true

            // Choose Map
            ScrollView {
                id: left_item_scrollview

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
                            markers: modelData.markers
                            hasMarkers: modelData.hasMarkers
                            mapIndex: index
                        }
                    }
                }
            }

            // Choose Markers
            Item {
                id: marker_list_menu

                Column {
                    id: marker_list_column
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    // Back
                    Row {
                        height: color_scheme.toolbarHeight
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 5

                        ToolBarIconButton {
                            fa_icon: FontAwesome.chevronLeft
                            height: color_scheme.toolbarHeight - 10
                            icon_color: color_scheme.textColor
                            darkBackground: color_scheme.dark

                            onClicked: left_swipe_view.decrementCurrentIndex()
                        }

                        Text {
                            text: qsTr("Map List")
                            color: color_scheme.textColor
                            font.bold: true
                            font.pointSize: 16
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width - x

                            MouseArea {
                                anchors.fill: parent
                                onClicked: left_swipe_view.decrementCurrentIndex()
                            }
                        }
                    }

                    Text {
                        id: markers_title_text
                        text: qsTr("Markers")
                        color: color_scheme.textColor
                        padding: 10
                        font.bold: true
                        font.pointSize: 16
                        anchors.left: parent.left
                        anchors.right: parent.right

                        // Add Marker
                        ToolBarIconButton {
                            fa_icon: FontAwesome.plus
                            height: color_scheme.toolbarHeight - 10
                            icon_color: color_scheme.textColor
                            darkBackground: color_scheme.dark

                            anchors.top: undefined
                            anchors.bottom: undefined
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter

                            onClicked: {
                                map_tool.addMarker(image.width / 2,
                                                   image.height / 2)
                            }
                        }
                    }
                }

                // List of Markers
                ScrollView {
                    id: marker_list_scroll
                    anchors.top: marker_list_column.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: 10
                    clip: true
                    contentWidth: -1

                    Column {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        spacing: 10

                        Repeater {
                            model: marker_repeater.model

                            Rectangle {
                                height: color_scheme.toolbarHeight
                                color: color_scheme.backgroundColor
                                border.width: marker_mouse_area.containsMouse ? 2 : 0
                                border.color: color_scheme.primaryButtonColor

                                anchors.left: parent.left
                                anchors.right: parent.right

                                Row {
                                    anchors.fill: parent
                                    spacing: 10
                                    padding: 5

                                    Text {
                                        text: modelData.icon
                                        font.family: FontAwesome.familySolid
                                        width: height
                                        color: modelData.color
                                        font.pixelSize: parent.height - 15
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                    }

                                    Text {
                                        text: modelData.name
                                        color: color_scheme.textColor
                                        font.pointSize: 12
                                        width: parent.width - parent.padding - x
                                        elide: Text.ElideRight
                                        verticalAlignment: Text.AlignVCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }

                                MouseArea {
                                    id: marker_mouse_area
                                    anchors.fill: parent
                                    hoverEnabled: true

                                    onClicked: {
                                        left_swipe_view.currentIndex = 2
                                        marker_name.text = modelData.name
                                        marker_description.text = modelData.description
                                        marker_icon.text = modelData.icon
                                        marker_icon.color = modelData.color

                                        map_tool.markerIndex = index
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Marker Details
            Item {
                id: marker_menu

                Column {
                    id: marker_menu_column
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    // Back
                    Row {
                        height: color_scheme.toolbarHeight
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 5

                        ToolBarIconButton {
                            fa_icon: FontAwesome.chevronLeft
                            height: color_scheme.toolbarHeight - 10
                            icon_color: color_scheme.textColor
                            darkBackground: color_scheme.dark

                            onClicked: left_swipe_view.decrementCurrentIndex()
                        }

                        Text {
                            text: qsTr("Marker List")
                            color: color_scheme.textColor
                            font.bold: true
                            font.pointSize: 16
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter

                            MouseArea {
                                anchors.fill: parent
                                onClicked: left_swipe_view.decrementCurrentIndex()
                            }
                        }
                    }

                    // Name and Icon
                    Row {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: 5
                        spacing: 10
                        padding: 5

                        Text {
                            id: marker_icon
                            font.family: FontAwesome.familySolid
                            width: height
                            font.pixelSize: parent.height - 15
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            id: marker_name
                            color: color_scheme.textColor
                            font.pointSize: 14
                            width: parent.width - parent.padding - x
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

                // Description
                ScrollView {
                    anchors.top: marker_menu_column.bottom
                    anchors.bottom: edit_marker_button.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 10

                    contentWidth: -1
                    clip: true

                    TextEdit {
                        id: marker_description
                        anchors.left: parent.left
                        anchors.right: parent.right

                        color: color_scheme.textColor
                        font.pointSize: 11
                        selectByMouse: true
                        wrapMode: Text.WordWrap
                        selectionColor: color_scheme.primaryButtonColor
                        readOnly: true
                    }
                }

                // Edit
                Rectangle {
                    id: edit_marker_button
                    height: color_scheme.toolbarHeight
                    color: color_scheme.backgroundColor
                    border.width: marker_edit_mouse_area.containsMouse ? 2 : 0
                    border.color: color_scheme.primaryButtonColor

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: delete_marker_button.top
                    anchors.margins: 10

                    Row {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Text {
                            text: FontAwesome.pen
                            font.family: FontAwesome.familySolid
                            width: height
                            color: color_scheme.textColor
                            font.pointSize: 14
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: qsTr("Edit")
                            color: color_scheme.textColor
                            font.pointSize: 14
                            width: parent.width - x
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    MouseArea {
                        id: marker_edit_mouse_area
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            marker_editor.markerName = marker_name.text
                            marker_editor.markerDescription = marker_description.text
                            marker_editor.markerIcon = marker_icon.text
                            marker_editor.markerColor = marker_icon.color
                            marker_editor.open()
                        }
                    }
                }

                // Delete
                Rectangle {
                    id: delete_marker_button
                    height: color_scheme.toolbarHeight
                    color: color_scheme.backgroundColor
                    border.width: marker_delete_mouse_area.containsMouse ? 2 : 0
                    border.color: color_scheme.primaryButtonColor

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: 10

                    Row {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Text {
                            text: FontAwesome.trash
                            font.family: FontAwesome.familySolid
                            width: height
                            color: color_scheme.textColor
                            font.pointSize: 14
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: qsTr("Delete")
                            color: color_scheme.textColor
                            font.pointSize: 14
                            width: parent.width - x
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    MouseArea {
                        id: marker_delete_mouse_area
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            marker_delete_rect.markerIndex = map_tool.markerIndex
                            marker_delete_rect.visible = true
                        }
                    }
                }
            }
        }
    }

    // Marker Label
    Label {
        id: marker_name_label
        z: 150
        anchors.horizontalCenter: flickable.horizontalCenter
        y: 20

        font.pointSize: 15
        color: "white"
        padding: 10

        visible: false

        background: Rectangle {
            color: "black"
            opacity: 0.8
        }
    }

    // Delete Marker ?
    Rectangle {
        id: marker_delete_rect
        anchors.horizontalCenter: flickable.horizontalCenter
        anchors.bottom: flickable.bottom
        anchors.bottomMargin: 20
        z: 150

        width: marker_delete_rect_row.width
        height: marker_delete_rect_row.height
        color: color_scheme.secondaryBackgroundColor

        visible: false

        property int markerIndex: -1

        Row {
            id: marker_delete_rect_row
            anchors.centerIn: parent
            spacing: 10
            padding: 10

            Text {
                text: qsTr("Delete?")
                font.pointSize: 12
                color: color_scheme.textColor
                anchors.verticalCenter: parent.verticalCenter
            }

            ToolBarIconButton {
                fa_icon: FontAwesome.check
                anchors.top: undefined
                anchors.bottom: undefined

                height: 30

                onClicked: {
                    marker_delete_rect.visible = false
                    console.log(marker_delete_rect.markerIndex)
                    map_tool.deleteMarker(marker_delete_rect.markerIndex)
                    left_swipe_view.currentIndex = 1
                }
            }

            ToolBarIconButton {
                fa_icon: FontAwesome.times
                anchors.top: undefined
                anchors.bottom: undefined

                height: 30

                onClicked: marker_delete_rect.visible = false
            }
        }
    }

    Flickable {
        id: flickable
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: left_item.visible ? left_item.right : parent.left
        anchors.right: maps_control_bar.left

        boundsBehavior: Flickable.DragOverBounds
        contentHeight: image_container.height
        contentWidth: image_container.width
        clip: true
        interactive: true

        property bool fitToScreenActive: true
        property real minZoom: 0.1
        property real maxZoom: 5
        property real zoomStep: 0.25

        //        onWidthChanged: {
        //            if (fitToScreenActive)
        //                fitToScreen()
        //        }

        //        onHeightChanged: {
        //            if (fitToScreenActive)
        //                fitToScreen()
        //        }
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

                Repeater {
                    id: marker_repeater
                    model: mapMarkerModel

                    MapMarker {
                        name: modelData.name
                        description: modelData.description
                        color: modelData.color
                        icon: modelData.icon
                        markerIndex: index

                        x: modelData.x
                        y: modelData.y
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
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: bottom_column.top

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
        }

        Column {
            id: bottom_column
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            // Show / Hide Marker menu
            Button {
                anchors.left: parent.left
                anchors.right: parent.right
                height: width
                hoverEnabled: true

                Text {
                    text: FontAwesome.mapMarkerAlt
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
                    left_swipe_view.currentIndex = 1
                    left_item.visible = true
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
