import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Rectangle {
    id: root
    color: palette.dark

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

            onDeleteButtonClicked: openDeleteDialog()

            onBack: {
                marker_menu_swipe.currentIndex = 0
            }
        }
    }
}
