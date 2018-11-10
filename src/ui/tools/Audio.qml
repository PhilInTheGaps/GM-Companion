import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

import gm.companion.audiotool 1.0
import gm.companion.platforms 1.0
import gm.companion.settingstool 1.0
import "./audio"
import "./audio/buttons"
import "./audio/editor"
import FontAwesome 2.0
import gm.companion.colorscheme 1.0

Page {
    ColorScheme {
        id: color_scheme
    }

    PlatformDetails {
        id: platform
    }

    function closeVolumePopup() {
        if (volume_dialog.opened) {
            volume_dialog.close()
        }
    }

    StackView {
        id: audio_stack
        anchors.fill: parent
        clip: true
        initialItem: audio_page

        background: Rectangle {
            color: color_scheme.backgroundColor
        }

        Page {
            id: audio_page
            background: Rectangle {
                color: color_scheme.backgroundColor
            }

            AudioTool {
                id: audio_tool

                function setCategory(category) {
                    setCurrentCategory(category)
                }

                function setScenario(scenario) {
                    setCurrentScenario(scenario)
                }

                function setMusic(element) {
                    music_element_name.text = element
                    playMusic(element)
                }

                function setSound(element) {
                    if (!isSoundPlayling(element)) {
                        playSound(element)
                        addSoundToSidebar(element)
                    }

                    audio_info_frame.visible = true
                }

                function stopSound(element) {
                    removeSound(element)

                    for (var i = sound_info_column.children.length; i > 1; i--) {
                        if (sound_info_column.children[i - 1].objectName === element) {
                            sound_info_column.children[i - 1].destroy()
                        }
                    }
                }

                function setRadio(element) {
                    playRadio(element)
                }

                function setSpotify(element) {
                    music_element_name.text = element
                    playSpotify(element)
                }

                function addSoundToSidebar(element) {
                    var component = Qt.createComponent(
                                "./audio/buttons/SoundButton.qml")
                    var button = component.createObject(sound_info_column, {
                                                            "element": element,
                                                            "element_icon": elementIcon(element),
                                                            "frame_width": audio_info_frame.width
                                                        })
                    button.clicked.connect(stopSound)
                }

                onCurrentProjectChanged: {
                    audio_project_structure.children = []
                    scenario_flow.children = []
                    audio_scroll_flow.children = []

                    var component = Qt.createComponent(
                                "./audio/buttons/CategoryButton.qml")
                    var categories = audio_tool.categories

                    for (var i = 0; i < categories.length; i++) {
                        var button = component.createObject(
                                    audio_project_structure, {
                                        "buttonText": categories[i]
                                    })
                        button.clicked.connect(setCategory)
                    }
                }

                onCurrentCategoryChanged: {
                    scenario_flow.children = []
                    audio_scroll_flow.children = []

                    var component = Qt.createComponent(
                                "./audio/buttons/ScenarioButton.qml")
                    var scenarios = audio_tool.scenarios

                    for (var i = 0; i < scenarios.length; i++) {
                        var button = component.createObject(scenario_flow, {
                                                                "buttonText": scenarios[i]
                                                            })
                        button.clicked.connect(setScenario)
                    }
                }

                onCurrentScenarioChanged: {
                    audio_busy_indicator.visible = true
                    audio_tool.findElements()
                }

                onElementsChanged: {

                    audio_scroll_flow.children = []
                    var component = Qt.createComponent(
                                "./audio/buttons/AudioButton.qml")

                    var elements = audio_tool.elements

                    for (var i = 0; i < elements.length; i++) {

                        var button = component.createObject(audio_scroll_flow, {
                                                                "element_name": elements[i],
                                                                "element_type": elementType(i),
                                                                "icon_path": elementIcons[i]
                                                            })

                        if (audio_tool.elementType(i) === 0) {
                            button.clicked.connect(setMusic)
                        } else if (audio_tool.elementType(i) === 1) {
                            button.clicked.connect(setSound)
                        } else if (audio_tool.elementType(i) === 2) {
                            button.clicked.connect(setRadio)
                        } else if (audio_tool.elementType(i) === 3) {
                            button.clicked.connect(setSpotify)
                        }
                    }

                    audio_busy_indicator.visible = false
                }

                onElementIconsChanged: {
                    console.log("Icons Changed!")

                    for (var i = 0; i < audio_scroll_flow.children.length; i++) {
                        audio_scroll_flow.children[i].icon_path = elementIcons[i]
                    }
                }

                onSongsChanged: {
                    for (var i = playlist_model.count; i > 0; i--) {
                        playlist_model.remove(i - 1)
                    }

                    for (var i = 0; i < songs.length; i++) {
                        playlist_model.append({
                                                  "name": songs[i]
                                              })
                    }
                }

                onCurrentSongIndexChanged: {
                    playlist_view.currentIndex = audio_tool.currentSongIndex
                    playlist_view.positionViewAtIndex(
                                playlist_view.currentIndex, ListView.Contain)
                }

                onMetaDataChanged: {
                    song_name_text.text = getSongName()
                    artist_text.text = getArtist()
                    album_text.text = getAlbum()
                    cover_image.source = getCoverArt()
                }
            }

            // Volume Dialog
            VolumeDialog {
                id: volume_dialog
                x: parent.width - width
                y: parent.height - audio_control_bar.height - height

                initialMusicVolume: 1
                initialSoundVolume: 0.5

                onMusicVolumeChanged: audio_tool.setMusicVolume(value)
                onSoundVolumeChanged: audio_tool.setSoundVolume(value)
            }

            // Bar at the bottom
            ToolBar {
                id: audio_control_bar
                height: platform.isAndroid ? parent.height / 12 : color_scheme.toolbarHeight
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                visible: true

                background: Rectangle {
                    color: color_scheme.menuColor
                }

                // Three Bars
                Button {
                    id: audio_project_structure_button
                    height: parent.height - 10
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                    hoverEnabled: true

                    background: Rectangle {
                        color: "transparent"
                    }

                    Text {
                        text: FontAwesome.bars
                        font.family: FontAwesome.familySolid
                        font.pixelSize: parent.height - 10
                        height: parent.height - 10
                        width: height
                        anchors.centerIn: parent
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    }

                    onClicked: audio_project_menu.visible ? audio_project_menu.visible = false : audio_project_menu.visible = true
                }

                // Volume Control Button
                Button {
                    id: volume_button
                    height: parent.height - 10
                    width: height
                    x: parent.width - width - 10 - playlist_button.width
                    anchors.verticalCenter: parent.verticalCenter
                    hoverEnabled: true

                    background: Rectangle {
                        color: "transparent"
                    }

                    Text {
                        text: FontAwesome.volumeUp
                        font.family: FontAwesome.familySolid
                        font.pixelSize: parent.height - 10
                        anchors.centerIn: parent
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    }

                    onClicked: volume_dialog.opened ? volume_dialog.close(
                                                          ) : volume_dialog.open()
                }

                // Playlist Button
                Button {
                    id: playlist_button
                    height: parent.height - 10
                    width: height
                    x: parent.width - width - 5
                    anchors.verticalCenter: parent.verticalCenter
                    hoverEnabled: true

                    background: Rectangle {
                        color: "transparent"
                    }

                    Text {
                        text: FontAwesome.listOl
                        font.family: FontAwesome.familySolid
                        font.pixelSize: parent.height - 10
                        anchors.centerIn: parent
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                    }

                    onClicked: audio_info_frame.visible ? audio_info_frame.visible
                                                          = false : audio_info_frame.visible = true
                }

                // Play Pause Skip
                Row {
                    spacing: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height - 10
                    y: 5

                    // Back
                    Button {
                        height: parent.height
                        width: parent.height
                        hoverEnabled: true

                        background: Rectangle {
                            color: "transparent"
                        }

                        Text {
                            text: FontAwesome.backward
                            font.family: FontAwesome.familySolid
                            font.pixelSize: parent.height - 10
                            anchors.centerIn: parent
                            color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                        }

                        onClicked: audio_tool.musicAgain()
                    }

                    // Play Pause
                    Button {
                        height: parent.height
                        width: parent.height
                        hoverEnabled: true

                        background: Rectangle {
                            color: "transparent"
                        }

                        Text {
                            id: audio_play_pause_icon
                            text: audio_tool.isPlaying ? FontAwesome.pauseCircle : FontAwesome.playCircle
                            font.family: FontAwesome.familySolid
                            font.pixelSize: parent.height
                            anchors.centerIn: parent
                            color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                        }

                        onClicked: audio_tool.musicPausePlay()
                    }

                    // Skip
                    Button {
                        height: parent.height
                        width: parent.height
                        hoverEnabled: true

                        background: Rectangle {
                            color: "transparent"
                        }

                        Text {
                            text: FontAwesome.forward
                            font.family: FontAwesome.familySolid
                            font.pixelSize: parent.height - 10
                            anchors.centerIn: parent
                            color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : "white"
                        }

                        onClicked: audio_tool.musicNext()
                    }
                }
            }

            // Main Row
            Row {
                id: audio_row
                anchors.fill: parent

                // Left Menu
                Column {
                    id: audio_project_menu
                    height: parent.height - audio_control_bar.height
                    width: platform.isAndroid ? parent.width / 5 : 175

                    spacing: platform.isAndroid ? 10 : 5
                    padding: 5

                    ComboBox {
                        id: audio_project_combo_box
                        width: parent.width - parent.padding
                        height: editor_button.height
                        model: audio_tool.projectList

                        onCurrentTextChanged: audio_tool.currentProject = currentText
                    }

                    Button {
                        id: editor_button

                        Text {
                            x: 10
                            text: FontAwesome.edit
                            font.family: FontAwesome.familySolid
                            font.pixelSize: parent.height - 15
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        text: qsTr("Editor")

                        width: parent.width - parent.padding
                        visible: !platform.isAndroid

                        onClicked: {
                            if (audio_stack.currentItem == audio_page) {
                                closeVolumePopup()
                                audio_page.visible = false
                                audio_editor.visible = true
                                audio_stack.pop(null)
                                audio_stack.push(audio_editor)
                            }
                        }

                        Component.onCompleted: {
                            if (platform.isAndroid)
                                height = width / 5
                        }
                    }

                    Text {
                        text: qsTr("Project Structure")
                        color: color_scheme.textColor
                    }

                    ScrollView {
                        id: audio_project_scroll_view
                        width: parent.width - parent.padding
                        height: audio_project_menu.height - y
                        clip: true
                        contentWidth: -1

                        Column {
                            id: audio_project_structure
                            width: audio_project_menu.width - audio_project_menu.padding
                            spacing: 5
                        }
                    }
                }

                // Mid Column
                Column {
                    id: audio_scenario_element_column
                    height: parent.height - audio_control_bar.height
                    width: {
                        if (audio_project_menu.visible) {
                            if (audio_info_frame.visible) {
                                parent.width - audio_project_menu.width - audio_info_frame.width
                            } else {
                                parent.width - audio_project_menu.width
                            }
                        } else {
                            if (audio_info_frame.visible) {
                                parent.width - audio_info_frame.width
                            } else {
                                parent.width
                            }
                        }
                    }

                    Flow {
                        id: scenario_flow
                        width: parent.width
                        spacing: 5
                        padding: 5
                    }

                    BusyIndicator {
                        id: audio_busy_indicator
                        visible: false

                        anchors.horizontalCenter: audio_scenario_element_column.horizontalCenter
                    }

                    ScrollView {
                        id: audio_scroll_view
                        height: parent.height - scenario_flow.height
                                - parent.topPadding * 2 - parent.spacing
                        width: parent.width

                        clip: true
                        contentWidth: -1

                        Flow {
                            id: audio_scroll_flow
                            padding: 5
                            topPadding: 0
                            spacing: 5

                            width: audio_scroll_view.width
                        }
                    }
                }

                // Right Menu
                Item {
                    id: audio_info_frame
                    height: parent.height - audio_control_bar.height
                    width: platform.isAndroid ? parent.width / 5 : 175
                    visible: true

                    Column {
                        anchors.fill: parent
                        spacing: 10
                        padding: 5

                        Column {
                            id: sound_info_column
                            width: parent.width - parent.padding * 2
                            spacing: 5

                            Text {
                                text: qsTr("Sounds")
                                color: color_scheme.textColor
                                font.bold: true
                            }
                        }

                        Text {
                            text: qsTr("No sounds playing.")
                            visible: sound_info_column.height < 20
                            color: color_scheme.textColor
                        }

                        Rectangle {
                            height: 1
                            anchors.left: parent.left
                            anchors.right: parent.right
                            color: color_scheme.dividerColor
                        }

                        Text {
                            id: music_element_name
                            text: qsTr("Music")
                            wrapMode: Text.WordWrap
                            width: parent.width - parent.padding * 2
                            color: color_scheme.textColor
                            font.bold: true
                        }

                        Text {
                            id: song_name_text
                            width: parent.width - parent.padding * 2
                            text: " - "
                            clip: true
                            elide: Text.ElideRight
                            color: color_scheme.textColor
                        }

                        Text {
                            id: artist_text
                            width: parent.width - parent.padding * 2
                            text: " - "
                            clip: true
                            elide: Text.ElideRight
                            color: color_scheme.textColor
                        }

                        Text {
                            id: album_text
                            width: parent.width - parent.padding * 2
                            text: " - "
                            clip: true
                            elide: Text.ElideRight
                            color: color_scheme.textColor
                        }

                        Image {
                            id: cover_image
                            visible: source != ""
                            width: parent.width - parent.padding * 2
                            sourceSize.width: width
                        }

                        Rectangle {
                            height: 1
                            anchors.left: parent.left
                            anchors.right: parent.right
                            color: color_scheme.dividerColor
                        }

                        // Playlist
                        ListView {
                            id: playlist_view
                            width: parent.width - parent.padding * 2
                            height: parent.height - y - parent.padding * 2

                            clip: true

                            highlight: Rectangle {
                                color: "lightgrey"
                            }

                            ScrollBar.vertical: ScrollBar {
                            }

                            model: PlaylistModel {
                                id: playlist_model
                            }

                            delegate: Item {
                                width: parent.width
                                height: playlist_text.height + 10

                                Text {
                                    id: playlist_text
                                    clip: true
                                    elide: Text.ElideRight
                                    text: name
                                    color: playlist_view.currentIndex === index ? color_scheme.playlistHiglightTextColor : color_scheme.textColor
                                    font.pointSize: 10
                                    anchors.centerIn: parent
                                    width: parent.width - 10
                                }

                                ToolTip {
                                    id: playlist_tooltip
                                    text: name
                                }

                                MouseArea {
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    onClicked: audio_tool.setMusicIndex(index)

                                    onEntered: playlist_tooltip.visible = true
                                    onExited: playlist_tooltip.visible = false
                                }
                            }
                        }
                    }
                }
            }
        }

        Editor {
            id: audio_editor
            visible: false

            onBackToTool: {
                if (audio_stack.currentItem == audio_editor) {
                    audio_editor.visible = false
                    audio_page.visible = true

                    audio_stack.pop(null)
                    audio_stack.push(audio_page)
                }
            }

            onProjectAdded: {
                audio_project_combo_box.model = audio_tool.projectList
            }
        }
    }
}
