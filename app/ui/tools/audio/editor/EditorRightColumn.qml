import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import "views"
import "views/element_properties"

Rectangle {
    id: element_info_item

    visible: audio_editor && audio_editor.currentElement
    color: palette.dark

    ElementPropertiesView {
        id: properties_view

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
    }

    ElementFileAddView {
        id: file_add_view
        anchors.top: properties_view.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
    }
}
