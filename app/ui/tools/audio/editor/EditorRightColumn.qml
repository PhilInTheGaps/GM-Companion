import QtQuick.Controls
import src
import "views"
import "views/element_properties"
import "../../../common"

Pane {
    id: root

    required property CustomFileDialog fileDialog
    required property IconFinderUnsplash unsplashDialog
    required property Dialog largeImageDialog

    visible: AudioTool.editor.currentElement

    ElementPropertiesView {
        id: properties_view

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        fileDialog: root.fileDialog
        unsplashDialog: root.unsplashDialog
        largeImageDialog: root.largeImageDialog
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
