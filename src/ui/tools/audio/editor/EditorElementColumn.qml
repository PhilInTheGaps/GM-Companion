import QtQuick 2.9
import QtQuick.Controls 2.2

ScrollView {
    id: root

    width: parent.width - parent.padding * 2
    height: parent.height - parent.spacing * 8 - parent.topPadding * 2
            - project_box.height - category_box.height - scenario_box.height
            - elements_text.height - sort_button.height
    clip: true

    // Fill element column with buttons
    function populate() {
        elements_column.children = []

        // Music lists
        for (var i = 0; i < editor_tool.getMusicLists().length; i++) {
            var component = Qt.createComponent(
                        "../buttons/EditorElementButton.qml")

            var button = component.createObject(elements_column, {
                                                    "element": editor_tool.getMusicLists(
                                                                   )[i],
                                                    "type": 0
                                                })
            button.clicked.connect(editor_tool.selectList)
            button.remove.connect(editor_tool.deleteList)
            button.moveUp.connect(moveElementUp)
            button.moveDown.connect(moveElementDown)
        }

        // Sound lists
        for (var i = 0; i < editor_tool.getSoundLists().length; i++) {
            var component = Qt.createComponent(
                        "../buttons/EditorElementButton.qml")

            var button = component.createObject(elements_column, {
                                                    "element": editor_tool.getSoundLists(
                                                                   )[i],
                                                    "type": 1
                                                })
            button.clicked.connect(editor_tool.selectList)
            button.remove.connect(editor_tool.deleteList)
            button.moveUp.connect(moveElementUp)
            button.moveDown.connect(moveElementDown)
        }

        // Radios
        for (var i = 0; i < editor_tool.getRadios().length; i++) {
            var component = Qt.createComponent(
                        "../buttons/EditorElementButton.qml")

            var button = component.createObject(elements_column, {
                                                    "element": editor_tool.getRadios(
                                                                   )[i],
                                                    "type": 2
                                                })
            button.clicked.connect(editor_tool.selectRadio)
            button.remove.connect(editor_tool.deleteList)
            button.moveUp.connect(moveElementUp)
            button.moveDown.connect(moveElementDown)
        }

        // Spotify
        for (var i = 0; i < editor_tool.getSpotifyPlaylists().length; i++) {
            var component = Qt.createComponent(
                        "../buttons/EditorElementButton.qml")

            var button = component.createObject(elements_column, {
                                                    "element": editor_tool.getSpotifyPlaylists(
                                                                   )[i],
                                                    "type": 3
                                                })
            button.clicked.connect(editor_tool.selectSpotifyList)
            button.remove.connect(editor_tool.deleteList)
            button.moveUp.connect(moveElementUp)
            button.moveDown.connect(moveElementDown)
        }
    }

    // Move elements in list
    function moveElementUp(element, type) {
        editor_tool.moveElement(element, type, -1)
    }

    function moveElementDown(element, type) {
        editor_tool.moveElement(element, type, 1)
    }

    Column {
        id: elements_column
        width: parent.parent.width
        spacing: 5
    }
}
