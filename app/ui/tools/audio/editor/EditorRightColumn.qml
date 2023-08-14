import QtQuick 2.9
import QtQuick.Controls 2.2
import IconFonts
import "views"
import "views/element_properties"

Pane {
    id: element_info_item

    visible: audio_editor && audio_editor.currentElement

    ElementPropertiesView {
        id: properties_view

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
    }

    ElementFileAddView {
        id: file_add_view
        anchors.top: properties_view.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 5
    }
}
