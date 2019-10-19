import QtQuick 2.10
import QtQuick.Controls 2.2
import FontAwesome 2.0
import "../../../components"
import "views"

Item {
    id: element_info_item

    anchors.left: parent.left
    anchors.right: parent.right
    visible: audio_editor.name != ""

    Column {
        id: column
        anchors.left: parent.left
        anchors.right: parent.right
        topPadding: 5
        spacing: 5

        ElementPropertiesView {
            id: properties_view
        }

        PlaybackModeView {
            id: playback_mode_view
        }
    }

    ElementFileAddView {
        id: file_add_view
        anchors.top: column.bottom
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
    }
}
