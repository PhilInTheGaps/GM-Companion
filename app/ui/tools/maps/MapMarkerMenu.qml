import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    color: palette.dark

    required property MapMarkerEditor markerEditor

    signal openDeleteDialog

    function openMarkerList() {
        marker_menu_swipe.currentIndex = 0
    }

    function openMarkerDetails() {
        marker_menu_swipe.currentIndex = 1
    }

    SwipeView {
        id: marker_menu_swipe
        anchors.fill: parent
        clip: true
        interactive: false

        // Choose Markers
        MapMarkerList {
            id: marker_list_menu

            onMarkerSelected: {
                marker_menu_swipe.currentIndex = 1
            }
        }

        // Marker Details
        MapMarkerDetails {
            id: marker_details

            markerEditor: root.markerEditor // qmllint disable incompatible-type

            onDeleteButtonClicked: root.openDeleteDialog()

            onBack: {
                marker_menu_swipe.currentIndex = 0
            }
        }
    }
}
