import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3

import gm.companion.charactertool 1.0
import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0
import "./characters"

Page {
    id: characters

    PlatformDetails {
        id: platform
    }

    CharacterTool {
        id: character_tool

        Component.onCompleted: updateCharacterList()

        onCharactersUpdated: {
            if (getActive()) {
                showActiveCharacters()
            } else {
                showInactiveCharacters()
            }
        }
    }

    function showActiveCharacters() {
        active_column.children = []

        for (var i = 0; i < character_tool.getActiveCharacterList(
                 ).length; i++) {
            var component = Qt.createComponent(
                        "./characters/CharacterButton.qml")
            var button = component.createObject(active_column, {
                                                    character_name: character_tool.getActiveCharacterList(
                                                                        )[i],
                                                    active: true
                                                })
            button.clicked.connect(loadCharacter)
            button.toggle_active_clicked.connect(toggleInactive)
        }
    }

    function showInactiveCharacters() {
        active_column.children = []

        for (var i = 0; i < character_tool.getInactiveCharacterList(
                 ).length; i++) {
            var component = Qt.createComponent(
                        "./characters/CharacterButton.qml")
            var button = component.createObject(active_column, {
                                                    character_name: character_tool.getInactiveCharacterList(
                                                                        )[i],
                                                    active: false
                                                })
            button.clicked.connect(loadCharacter)
            button.toggle_active_clicked.connect(toggleInactive)
        }
    }

    function loadCharacter(character) {
        var images = character_tool.getImages(character)
        character_tool.loadButtonTexts(character)
        bottom_row.children = []

        for (var i = 0; i < images.length; i++) {
            var component = Qt.createComponent("./characters/PageButton.qml")
            var button = component.createObject(bottom_row, {
                                                    display_text: character_tool.getButtonText(
                                                                      i),
                                                    source: images[i]
                                                })
            button.clicked.connect(loadImage)
        }

        loadImage(images[0])
    }

    function loadImage(source) {
        character_image.source = "file:///" + source
    }

    function toggleInactive(character, active) {
        if (active) {
            character_tool.setCharacterInactive(character)
        } else {
            character_tool.setCharacterActive(character)
        }
    }

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Column {
        anchors.fill: parent

        Rectangle {
            id: top_rect
            color: color_scheme.menuColor
            height: top_left_row.height
            anchors.left: parent.left
            anchors.right: parent.right

            Row {
                id: top_left_row
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5
                padding: 5

                Button {
                    id: active_button
                    text: qsTr("Active")
                    height: platform.isAndroid ? width / 3 : 40
                    onClicked: {
                        character_tool.setActive(true)
                        showActiveCharacters()
                    }
                }

                Button {
                    id: inactive_button
                    text: qsTr("Inactive")
                    height: platform.isAndroid ? width / 3 : 40
                    onClicked: {
                        showInactiveCharacters()
                        character_tool.setActive(false)
                    }
                }
            }

            Row {
                id: top_right_row
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5
                padding: 5

                Button {
                    Image {
                        source: "/icons/menu/plus_sign_dark.png"
                        width: parent.height / 1.5
                        height: width
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    width: active_button.height
                    height: width

                    onClicked: {

                        character_image.height *= 2
                        character_image.width *= 2

                        character_flickable.contentY = character_flickable.contentHeight
                                / 2 - character_flickable.height / 2
                        character_flickable.contentX = character_flickable.contentWidth
                                / 2 - character_flickable.width / 2
                    }
                }

                Button {
                    Image {
                        source: "/icons/menu/vertical_bar_dark.png"
                        width: parent.height / 1.5
                        height: width
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    width: active_button.height
                    height: width

                    onClicked: {
                        character_image.height *= 0.5
                        character_image.width *= 0.5

                        character_flickable.contentY = character_flickable.contentHeight
                                / 2 - character_flickable.height / 2
                        character_flickable.contentX = character_flickable.contentWidth
                                / 2 - character_flickable.width / 2
                    }
                }

                Button {
                    Image {
                        source: "/icons/menu/ring_arrow_dark.png"
                        width: parent.height / 1.5
                        height: width

                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        mirror: true
                    }

                    width: active_button.height
                    height: width

                    onClicked: {
                        character_image.width = character_flickable.width
                        character_image.height = character_flickable.height
                    }
                }
            }
        }

        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height - bottom_rect.height - top_rect.height

            spacing: 5
            padding: 5

            ScrollView {
                id: character_scrollview
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                width: platform.isAndroid ? parent.width / 5 : 150
                clip: true

                Column {
                    id: active_column
                    width: character_scrollview.width
                    spacing: 5
                    topPadding: 5
                    bottomPadding: 5
                }
            }

            Rectangle {
                id: mid_column
                width: (parent.width - character_scrollview.width
                        - parent.spacing - parent.leftPadding * 2)
                height: parent.height - parent.topPadding * 2

                Flickable {
                    id: character_flickable
                    anchors.fill: parent
                    clip: true
                    interactive: true

                    contentWidth: character_image.width
                    contentHeight: character_image.height

                    PinchArea {
                        width: Math.max(character_flickable.contentWidth,
                                        character_flickable.width)
                        height: Math.max(character_flickable.contentHeight,
                                         character_flickable.height)

                        property real initialWidth
                        property real initialHeight

                        onPinchStarted: {
                            initialWidth = character_flickable.contentWidth
                            initialHeight = character_flickable.contentHeight
                        }

                        onPinchUpdated: {
                            // adjust content pos due to drag
                            character_flickable.contentX += pinch.previousCenter.x - pinch.center.x
                            character_flickable.contentY += pinch.previousCenter.y - pinch.center.y

                            // resize content
                            character_flickable.resizeContent(
                                        initialWidth * pinch.scale,
                                        initialHeight * pinch.scale,
                                        pinch.center)

                            character_image.width = character_flickable.contentWidth
                            character_image.height = character_flickable.contentHeight
                        }

                        onPinchFinished: {
                            // Move its content within bounds.
                            character_flickable.returnToBounds()
                        }
                    }

                    Image {
                        id: character_image
                        width: mid_column.width
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }

        Rectangle {
            id: bottom_rect
            color: color_scheme.menuColor
            height: bottom_row.height
            anchors.left: parent.left
            anchors.right: parent.right

            Row {
                id: bottom_row
                anchors.centerIn: parent
                padding: 5
                spacing: 5
            }
        }
    }
}
