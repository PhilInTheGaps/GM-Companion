import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

import gm.companion.audiotool 1.0
import gm.companion.platforms 1.0
import gm.companion.settingstool 1.0
import "./audio"

import gm.companion.colorscheme 1.0

Page {
    ColorScheme {
        id: color_scheme
    }

    SwipeView {
        id: audio_swipe
        anchors.fill: parent
        interactive: false

        background: Rectangle {
            color: color_scheme.backgroundColor
        }

        Page {
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

                function addSoundToSidebar(element) {
                    var component = Qt.createComponent(
                                "./audio/SoundButton.qml")
                    var button = component.createObject(sound_info_column, {
                                                            x: 0,
                                                            y: 0,
                                                            element: element,
                                                            element_icon: elementIcon(element),
                                                            frame_width: audio_info_frame.width
                                                        })
                    button.clicked.connect(stopSound)
                }

                onCurrentProjectChanged: {
                    audio_project_structure.children = []
                    scenario_flow.children = []
                    audio_scroll_flow.children = []

                    var component = Qt.createComponent(
                                "./audio/CategoryButton.qml")
                    var categories = audio_tool.categories

                    for (var i = 0; i < categories.length; i++) {
                        var button = component.createObject(
                                    audio_project_structure, {
                                        x: 0,
                                        y: 0,
                                        category: categories[i],
                                        buttonId: categories[i],
                                        max_width: audio_project_menu.width - 5
                                    })
                        button.clicked.connect(setCategory)
                    }
                }

                onCurrentCategoryChanged: {
                    scenario_flow.children = []
                    audio_scroll_flow.children = []

                    var component = Qt.createComponent(
                                "./audio/ScenarioButton.qml")
                    var scenarios = audio_tool.scenarios

                    for (var i = 0; i < scenarios.length; i++) {
                        var button = component.createObject(scenario_flow, {
                                                                x: 0,
                                                                y: 0,
                                                                scenario: scenarios[i],
                                                                buttonId: scenarios[i],
                                                                max_width: 150
                                                            })
                        button.clicked.connect(setScenario)
                    }
                }

                onCurrentScenarioChanged: {
                    audio_busy_indicator.visible = true

                    audio_scroll_flow.children = []
                    var component = Qt.createComponent(
                                "./audio/AudioButton.qml")

                    audio_tool.findElements()

                    var elements = audio_tool.elements

                    for (var i = 0; i < elements.length; i++) {

                        var button = component.createObject(audio_scroll_flow, {
                                                                x: 0,
                                                                y: 0,
                                                                element_name: elements[i],
                                                                element_type: elementType(i),
                                                                icon_path: elementIcon(elements[i])
                                                            })

                        if (audio_tool.elementType(i) === 0) {
                            button.clicked.connect(setMusic)
                        } else if (audio_tool.elementType(i) === 1) {
                            button.clicked.connect(setSound)
                        } else if (audio_tool.elementType(i) === 2) {
                            button.clicked.connect(setRadio)
                        }
                    }

                    audio_busy_indicator.visible = false
                }

                onIsPlayingChanged: {
                    audio_play_pause_icon.source
                            = isPlaying ? "/icons/media/pause.png" : "/icons/media/play.png"
                }

                onSongsChanged: {
                    for (var i = playlist_model.count; i > 0; i--) {
                        playlist_model.remove(i - 1)
                    }

                    for (var i = 0; i < songs.length; i++) {
                        playlist_model.append({
                                                  name: songs[i]
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
                }
            }

            ToolBar {
                id: audio_control_bar
                width: parent.width
                height: parent.height / 18
                anchors.bottom: parent.bottom
                visible: true

                background: Rectangle {
                    color: color_scheme.menuColor
                }

                Button {
                    anchors.left: parent.left
                    height: parent.height - 10
                    width: height
                    y: 5

                    id: audio_project_structure_button

                    Image {
                        source: "/icons/menu/three_bars_dark.png"
                        width: parent.height / 1.5
                        height: width

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    onClicked: audio_project_menu.visible ? audio_project_menu.visible = false : audio_project_menu.visible = true
                }

                Column {
                    width: parent.width / 5
                    height: audio_control_bar.height
                    anchors.verticalCenter: parent.verticalCenter
                    x: audio_project_structure_button.width + 10

                    padding: 5
                    spacing: 5

                    Row {
                        width: parent.width
                        height: (parent.height - parent.spacing
                                 - parent.topPadding - parent.bottomPadding) / 2
                        spacing: 5

                        Image {
                            id: music_icon
                            source: "/icons/media/music_bright.png"
                            height: parent.height
                            width: height
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Slider {
                            width: parent.width - music_icon.width - parent.spacing
                            height: parent.height
                            from: 0
                            value: 1.5
                            to: 2

                            handle: Rectangle {
                                x: parent.leftPadding + parent.visualPosition
                                   * (parent.availableWidth - width)
                                y: parent.topPadding + parent.availableHeight / 2 - height / 2

                                implicitWidth: parent.height
                                implicitHeight: parent.height
                                radius: 13
                                color: parent.pressed ? "#f0f0f0" : "#f6f6f6"
                                border.color: "#bdbebf"
                            }

                            onValueChanged: audio_tool.setMusicVolume(value)
                        }
                    }

                    Row {
                        width: parent.width
                        height: (parent.height - parent.spacing
                                 - parent.topPadding - parent.bottomPadding) / 2
                        spacing: 5

                        Image {
                            id: sound_icon
                            source: "/icons/media/sound_bright.png"
                            height: parent.height
                            width: height
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Slider {
                            width: parent.width - sound_icon.width - parent.spacing
                            height: parent.height
                            from: 0
                            value: 0.5
                            to: 2

                            handle: Rectangle {
                                x: parent.leftPadding + parent.visualPosition
                                   * (parent.availableWidth - width)
                                y: parent.topPadding + parent.availableHeight / 2 - height / 2

                                implicitWidth: parent.height
                                implicitHeight: parent.height
                                radius: 13
                                color: parent.pressed ? "#f0f0f0" : "#f6f6f6"
                                border.color: "#bdbebf"
                            }

                            onValueChanged: audio_tool.setSoundVolume(value)
                        }
                    }
                }

                Button {
                    anchors.right: parent.right

                    height: parent.height - 10
                    width: height
                    y: 5

                    Image {
                        source: "/icons/media/playlist.png"
                        width: parent.height / 1.5
                        height: width

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    onClicked: audio_info_frame.visible ? audio_info_frame.visible
                                                          = false : audio_info_frame.visible = true
                }

                Row {
                    spacing: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height - 10
                    y: 5

                    Button {
                        height: parent.height
                        width: parent.height

                        Image {
                            source: "/icons/media/seekBack.png"
                            width: parent.height / 1.5
                            height: width

                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        onClicked: audio_tool.musicAgain()
                    }

                    Button {
                        height: parent.height
                        width: parent.height

                        Image {
                            id: audio_play_pause_icon
                            source: "/icons/media/play.png"
                            width: parent.height / 1.5
                            height: width

                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        onClicked: audio_tool.musicPausePlay()
                    }

                    Button {
                        height: parent.height
                        width: parent.height

                        Image {
                            source: "/icons/media/seekForward.png"
                            width: parent.height / 1.5
                            height: width

                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        onClicked: audio_tool.musicNext()
                    }
                }
            }

            Row {
                id: audio_row
                anchors.fill: parent

                Column {
                    id: audio_project_menu
                    height: parent.height - audio_control_bar.height
                    width: platform.isAndroid ? parent.width / 2 : 150

                    spacing: 5
                    padding: 5

                    Text {
                        text: qsTr("Project")
                    }

                    ComboBox {
                        id: audio_project_combo_box
                        width: parent.width - parent.padding
                        model: audio_tool.projectList

                        onCurrentTextChanged: audio_tool.currentProject = currentText
                    }

                    Button {
                        text: qsTr("Editor")
                        width: parent.width - parent.padding

                        onClicked: audio_swipe.currentIndex = 1
                    }

                    Text {
                        text: qsTr("Project Structure")
                    }

                    ScrollView {
                        id: audio_project_scroll_view
                        width: parent.width - parent.padding
                        height: audio_project_menu.height - y

                        Column {
                            id: audio_project_structure
                            spacing: 5
                        }
                    }
                }

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
                        anchors.verticalCenter: audio_scenario_element_column.verticalCenter
                    }

                    ScrollView {
                        id: audio_scroll_view
                        height: parent.height - scenario_flow.height
                                - parent.topPadding * 2 - parent.spacing
                        width: parent.width

                        clip: true

                        Flow {
                            id: audio_scroll_flow
                            padding: 5
                            spacing: 5

                            width: audio_scroll_view.width
                        }
                    }
                }

                Frame {
                    id: audio_info_frame
                    height: parent.height - audio_control_bar.height
                    width: 200
                    visible: false

                    Column {
                        anchors.fill: parent
                        spacing: 10

                        Column {
                            id: sound_info_column
                            width: parent.width
                            spacing: 5

                            Text {
                                text: qsTr("Sounds")
                            }
                        }

                        Text {
                            id: music_element_name
                            text: qsTr("No Music Selected")
                            wrapMode: Text.WordWrap
                            width: parent.width
                        }

                        Frame {
                            id: music_metadata_column
                            width: parent.width
                            spacing: 5
                            padding: 5

                            Column {
                                width: parent.width

                                Text {
                                    id: song_name_text
                                    width: parent.width - parent.padding * 2
                                    text: ""
                                    clip: true
                                    elide: Text.ElideRight
                                }

                                Text {
                                    id: artist_text
                                    width: parent.width - parent.padding * 2
                                    text: ""
                                    clip: true
                                    elide: Text.ElideRight
                                }

                                Text {
                                    id: album_text
                                    width: parent.width - parent.padding * 2
                                    text: ""
                                    clip: true
                                    elide: Text.ElideRight
                                }
                            }
                        }

                        ListView {
                            id: playlist_view
                            width: parent.width
                            height: parent.height - sound_info_column.height
                                    - music_element_name.height - parent.spacing
                                    * 3 - music_metadata_column.height
                            clip: true

                            highlight: Rectangle {
                                color: "lightgrey"
                            }

                            model: PlaylistModel {
                                id: playlist_model
                            }

                            delegate: Text {
                                width: parent.width
                                clip: true
                                elide: Text.ElideRight
                                text: name

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: audio_tool.setMusicIndex(index)
                                }
                            }
                        }
                    }
                }
            }
        }

        Editor {
            id: audio_editor

            onBackToTool: audio_swipe.currentIndex = 0
        }
    }
}
