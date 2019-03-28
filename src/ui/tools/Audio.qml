import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Window 2.2

import "./audio"
import "./audio/buttons"
import "./audio/editor"
import "../components"
import FontAwesome 2.0

Page {
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

            // Volume Dialog
            VolumeDialog {
                id: volume_dialog
                x: parent.width - width
                y: parent.height - audio_control_bar.height - height

                initialMusicVolume: 0.25
                initialSoundVolume: 0.25
            }

            // Bar at the top
            Rectangle {
                id: top_bar
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: color_scheme.toolbarHeight
                color: color_scheme.menuColor

                // Left side
                Row {
                    id: top_left_row
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.margins: 5
                    width: audio_project_menu.width
                    spacing: 5

                    // Project ComboBox
                    ToolBarComboBox {
                        id: audio_project_combo_box
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: parent.width - editor_button.width - parent.spacing

                        model: audio_tool.projectNames

                        onCurrentTextChanged: audio_tool.setCurrentProject(
                                                  currentIndex)
                    }

                    // Open Editor Button
                    ControlBarButton {
                        id: editor_button
                        fa_icon: FontAwesome.edit
                        anchors.margins: 2

                        onClicked: {
                            if (audio_stack.currentItem == audio_page) {
                                closeVolumePopup()
                                audio_page.visible = false
                                audio_editor_page.visible = true
                                audio_stack.pop(null)
                                audio_stack.push(audio_editor_page)
                            }
                        }
                    }
                }
            }

            // Bar at the bottom
            ControlBar {
                id: audio_control_bar
            }

            // Categories
            ScrollView {
                id: audio_project_menu
                anchors.top: top_bar.bottom
                anchors.bottom: audio_control_bar.top
                anchors.bottomMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5

                width: platform.isAndroid ? parent.width / 5 : 175
                clip: true
                contentWidth: -1
                contentHeight: audio_project_structure.implicitHeight

                Column {
                    id: audio_project_structure
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 5
                    topPadding: 5

                    Repeater {
                        id: category_repeater

                        model: audio_tool.categoryNames

                        CategoryButton {
                            buttonText: modelData

                            onClicked: audio_tool.setCurrentCategory(buttonText)
                        }
                    }
                }
            }

            // Mid Column
            Item {
                id: audio_scenario_element_column
                anchors.top: parent.top
                anchors.bottom: audio_control_bar.top
                anchors.bottomMargin: 5
                anchors.left: audio_project_menu.visible ? audio_project_menu.right : parent.left
                anchors.right: parent.right

                // Scenarios
                Item {
                    id: scenario_container
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 5

                    height: scenario_flow.height

                    Rectangle {
                        anchors.fill: parent
                        color: color_scheme.menuColor
                    }

                    Flow {
                        id: scenario_flow
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Repeater {
                            id: scenario_repeater

                            model: audio_tool.scenarioNames

                            ScenarioButton {
                                labeltext: modelData

                                onClicked: audio_tool.setCurrentScenario(
                                               labeltext)
                            }
                        }
                    }
                }

                // Elements
                ScrollView {
                    id: audio_scroll_view
                    anchors.top: scenario_container.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: sound_bar.visible ? sound_bar.left : (audio_info_frame.visible ? audio_info_frame.left : parent.right)

                    anchors.topMargin: 5

                    clip: true
                    contentWidth: -1
                    contentHeight: audio_scroll_flow.implicitHeight

                    Flow {
                        id: audio_scroll_flow
                        leftPadding: 5
                        rightPadding: 5
                        spacing: 5

                        width: audio_scroll_view.width

                        readonly property int min_width: 175
                        readonly property int count_per_row: parent ? Math.floor(
                                                                          parent.width
                                                                          / min_width) : 1
                        readonly property int button_width: platform.isAndroid ? (width / 8) : ((width - spacing * (count_per_row - 1) - leftPadding * 2) / count_per_row)

                        Repeater {
                            id: element_repeater
                            model: elementModel

                            AudioButton {
                                element_name: modelData.name
                                element_type: modelData.type
                                icon_path: modelData.icon
                                has_icon: modelData.hasIcon
                                width: audio_scroll_flow.button_width

                                onClicked: audio_tool.playElement(
                                               modelData.name, modelData.type)
                            }
                        }
                    }
                }

                // Sound Buttons
                Rectangle {
                    id: sound_bar
                    color: "transparent"

                    visible: !soundModel.isEmpty
                    anchors.top: scenario_container.bottom
                    anchors.bottom: parent.bottom
                    anchors.right: audio_info_frame.visible ? audio_info_frame.left : parent.right
                    anchors.topMargin: 5
                    anchors.rightMargin: 5
                    width: audio_project_menu.width

                    Column {
                        id: sound_flow
                        anchors.fill: parent
                        spacing: 5

                        Repeater {
                            id: sound_repeater
                            model: soundModel

                            SoundButton {
                                element: modelData.name
                                element_icon: modelData.icon
                                has_icon: modelData.hasIcon
                                anchors.left: parent.left
                                anchors.right: parent.right
                            }
                        }
                    }
                }

                // Info
                Item {
                    id: audio_info_frame
                    anchors.top: scenario_container.bottom
                    anchors.topMargin: 5
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    width: platform.isAndroid ? parent.width / 5 : 175
                    visible: true

                    // Meta Data
                    MetaDataDisplay {
                        id: meta_data_display
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                    }

                    Rectangle {
                        id: right_spacer
                        height: 1
                        anchors.top: meta_data_display.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.topMargin: 5
                        color: color_scheme.dividerColor
                    }

                    // Playlist
                    ListView {
                        id: playlist_view
                        anchors.top: right_spacer.bottom
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.topMargin: 5
                        height: parent.height - y - parent.padding * 2

                        clip: true

                        highlight: Rectangle {
                            color: "lightgrey"
                        }

                        ScrollBar.vertical: ScrollBar {
                        }

                        model: audio_tool.songs
                        currentIndex: audio_tool.index

                        delegate: Item {
                            width: parent.width
                            height: playlist_text.height + 10

                            Text {
                                id: playlist_text
                                clip: true
                                elide: Text.ElideRight
                                text: modelData
                                color: playlist_view.currentIndex === index ? color_scheme.playlistHiglightTextColor : color_scheme.textColor
                                font.pointSize: 10
                                anchors.centerIn: parent
                                width: parent.width - 10
                            }

                            ToolTip {
                                id: playlist_tooltip
                                text: modelData
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

        Editor {
            id: audio_editor_page
            visible: false

            onBackToTool: {
                if (audio_stack.currentItem == audio_editor_page) {
                    audio_editor_page.visible = false
                    audio_page.visible = true

                    audio_stack.pop(null)
                    audio_stack.push(audio_page)
                }
            }
        }
    }
}
