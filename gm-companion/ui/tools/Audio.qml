import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import gm.companion.audiotool 1.0
import gm.companion.platforms 1.0
import "./audio"

Page {
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

        onCurrentProjectChanged: {
            audio_project_structure.children = []
            scenario_flow.children = []
            audio_scroll_flow.children = []

            var component = Qt.createComponent("./audio/CategoryButton.qml")
            var categories = audio_tool.categories

            for (var i = 0; i < categories.length; i++) {
                var button = component.createObject(audio_project_structure, {
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

            var component = Qt.createComponent("./audio/ScenarioButton.qml")
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
            var component = Qt.createComponent("./audio/AudioButton.qml")

            audio_tool.findElements()

            var elements = audio_tool.elements

            for (var i = 0; i < elements.length; i++) {

                var button = component.createObject(audio_scroll_flow, {
                                                        x: 0,
                                                        y: 0,
                                                        element_name: elements[i],
                                                        icon_path: elementIcon(
                                                                       elements[i])
                                                    })

                button.clicked.connect(setMusic)
            }

            audio_busy_indicator.visible = false
        }

        onIsPlayingChanged: {
            audio_play_pause_icon.source
                    = isPlaying ? "/icons/media/pause.png" : "/icons/media/play.png"
        }
    }

    ToolBar {
        id: audio_control_bar
        width: parent.width
        height: 50
        anchors.bottom: parent.bottom
        visible: true

        Button {
            anchors.left: parent.left
            height: parent.height
            width: parent.height
            id: audio_project_structure_button

            Image {
                source: "/icons/menu/three_bars_dark.png"
                height: parent.height * 0.8
                width: parent.width * 0.8

                sourceSize.width: width
                sourceSize.height: height

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: audio_project_menu.visible ? audio_project_menu.visible
                                                    = false : audio_project_menu.visible = true
        }

        Column {
            width: parent.width / 5
            anchors.verticalCenter: parent.verticalCenter
            x: audio_project_structure_button.width + 10

            Row {
                width: parent.width
                spacing: 5

                Image {
                    id: music_icon
                    source: "/icons/media/music.png"
                    height: audio_control_bar.height / 2.5
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                }

                Slider {
                    width: parent.width - music_icon.width - parent.spacing
                    value: 1.5
                    maximumValue: 2

                    onValueChanged: audio_tool.setMusicVolume(value)
                }
            }

            Row {
                width: parent.width
                spacing: 5

                Image {
                    id: sound_icon
                    source: "/icons/media/sound.png"
                    height: audio_control_bar.height / 2.5
                    width: height
                    anchors.verticalCenter: parent.verticalCenter
                }

                Slider {
                    width: parent.width - sound_icon.width - parent.spacing
                    value: 0.5
                    maximumValue: 2

                    onValueChanged: audio_tool.setSoundVolume(value)
                }
            }
        }

        Button {
            anchors.right: parent.right

            height: parent.height
            width: parent.height

            Image {
                source: "/icons/media/playlist.png"
                height: parent.height * 0.8
                width: parent.width * 0.8

                sourceSize.width: width
                sourceSize.height: height

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: audio_playlist_view.visible ? audio_playlist_view.visible
                                                     = false : audio_playlist_view.visible = true
        }

        Row {
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height

            Button {
                height: parent.height
                width: parent.height

                Image {
                    source: "/icons/media/seekBack.png"
                    height: parent.height * 0.8
                    width: parent.width * 0.8

                    sourceSize.width: width
                    sourceSize.height: height

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
                    height: parent.height * 0.8
                    width: parent.width * 0.8

                    sourceSize.width: width
                    sourceSize.height: height

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
                    height: parent.height * 0.8
                    width: parent.width * 0.8

                    sourceSize.width: width
                    sourceSize.height: height

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
            width: platform.isAndroid ? parent.width / 5 : 150

            spacing: 5
            padding: 5

            Text {
                text: qsTr("Project")
            }

            ComboBox {
                id: audio_project_combo_box
                width: parent.width - parent.padding
                model: audio_tool.projectList
            }

            Button {
                text: qsTr("Load")
                width: parent.width - parent.padding

                onClicked: audio_tool.currentProject = audio_project_combo_box.currentText
            }

            Button {
                text: qsTr("Editor")
                width: parent.width - parent.padding
            }

            Text {
                text: qsTr("Project Structure")
            }

            ScrollView {
                id: audio_project_scroll_view
                width: parent.width - parent.padding
                height: audio_project_menu.height - y
                flickableItem.interactive: true

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
                    if (audio_playlist_view.visible) {
                        parent.width - audio_project_menu.width - audio_playlist_view.width
                    } else {
                        parent.width - audio_project_menu.width
                    }
                } else {
                    if (audio_playlist_view.visible) {
                        parent.width - audio_playlist_view.width
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
                width: parent.width

                flickableItem.interactive: true

                Flow {
                    id: audio_scroll_flow
                    padding: 5
                    spacing: 5

                    width: audio_scroll_view.viewport.width
                }
            }
        }

        Frame {
            id: audio_playlist_view
            height: parent.height - audio_control_bar.height
            width: 150
            visible: false

            ListView {
                anchors.fill: parent
                currentIndex: 0

                Column {
                    anchors.fill: parent
                    Text {
                        id: music_element_name
                        text: "Test" //qsTr("No Music Selected")
                    }
                }
            }
        }
    }
}
