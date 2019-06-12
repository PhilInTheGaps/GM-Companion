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
        volume_dialog.visible = false
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
                z: 100

                visible: false

                initialMusicVolume: 0.25
                initialSoundVolume: 0.25
            }

            // Bar at the top
            Rectangle {
                id: top_bar
                anchors.left: parent.left
                width: left_menu.width
                anchors.top: parent.top
                height: color_scheme.toolbarHeight
                color: color_scheme.secondaryBackgroundColor
                visible: left_menu.visible

                // Left side
                Row {
                    id: top_left_row
                    anchors.margins: 5
                    anchors.leftMargin: 0
                    anchors.fill: parent
                    spacing: 5

                    // Project ComboBox
                    ToolBarComboBox {
                        id: audio_project_combo_box
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: parent.width - editor_button.width - parent.spacing
                        textColor: color_scheme.textColor
                        darkBackground: color_scheme.dark

                        textItem.leftPadding: 0
                        model: audio_tool.projectNames

                        onCurrentTextChanged: audio_tool.setCurrentProject(
                                                  currentIndex)
                    }

                    // Open Editor Button
                    ControlBarButton {
                        id: editor_button
                        fa_icon: FontAwesome.edit
                        anchors.margins: 2
                        icon_color: color_scheme.textColor
                        darkBackground: color_scheme.dark

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
            Rectangle {
                id: left_menu
                color: color_scheme.secondaryBackgroundColor

                anchors.top: top_bar.bottom
                anchors.bottom: audio_control_bar.top
                anchors.left: parent.left
                width: platform.isAndroid ? parent.width / 5 : 175

                ScrollView {
                    id: audio_project_menu

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: sound_column.top
                    anchors.margins: 5

                    clip: true
                    contentWidth: -1
                    contentHeight: audio_project_structure.implicitHeight

                    Column {
                        id: audio_project_structure
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Repeater {
                            id: category_repeater

                            model: audio_tool.categoryNames

                            CategoryButton {
                                buttonText: modelData

                                onClicked: audio_tool.setCurrentCategory(
                                               buttonText)
                            }
                        }
                    }
                }

                Column {
                    id: sound_column
                    anchors.bottom: cover_image.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 5
                    bottomPadding: 5

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

                Image {
                    id: cover_image
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    visible: source != ""
                    sourceSize.width: width
                    source: audio_tool.cover
                }
            }

            // Mid Column
            Item {
                id: audio_scenario_element_column
                anchors.top: parent.top
                anchors.bottom: audio_control_bar.top
                anchors.bottomMargin: 5
                anchors.left: left_menu.visible ? left_menu.right : parent.left
                anchors.right: audio_info_frame.visible ? audio_info_frame.left : parent.right

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
                        color: color_scheme.backgroundColor
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
                    anchors.right: parent.right

                    anchors.topMargin: 5

                    clip: true
                    contentWidth: -1
                    contentHeight: audio_element_column.implicitHeight

                    Column {
                        id: audio_element_column
                        anchors.margins: 5
                        anchors.left: parent.left
                        anchors.right: parent.right
                        spacing: 10

                        readonly property int min_width: 170
                        readonly property int count_per_row: parent ? Math.floor(
                                                                          parent.width
                                                                          / min_width) : 1
                        readonly property int button_width: platform.isAndroid ? (width / 8) : ((width - 5 * (count_per_row - 1)) / count_per_row)

                        Repeater {
                            model: elementModel

                            Column {
                                width: audio_element_column.width
                                spacing: 10

                                Text {
                                    id: subscenario_text
                                    text: modelData.name()
                                    visible: text != ""
                                    color: color_scheme.textColor
                                    font.bold: true
                                    font.pointSize: 15
                                }

                                Flow {
                                    id: audio_scroll_flow

                                    spacing: 5
                                    anchors.left: parent.left
                                    anchors.right: parent.right

                                    Repeater {
                                        id: element_repeater
                                        model: modelData

                                        AudioButton {
                                            element_name: modelData.name
                                            element_type: modelData.type
                                            subscenario_name: subscenario_text.text
                                            icon_path: modelData.icon
                                            has_icon: modelData.hasIcon
                                            width: audio_element_column.button_width

                                            onClicked: audio_tool.playElement(
                                                           modelData.name,
                                                           modelData.type,
                                                           subscenario_name)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // Search
                Rectangle {
                    height: color_scheme.toolbarHeight
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right

                    // Search Button
                    Rectangle {
                        id: search_button
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        width: height
                        radius: search_field.visible ? 0 : width / 2

                        color: color_scheme.primaryButtonColor

                        Text {
                            font.family: FontAwesome.familySolid
                            text: search_field.visible ? FontAwesome.times : FontAwesome.search
                            anchors.centerIn: parent
                            color: search_mouse_area.containsMouse ? "grey" : color_scheme.buttonTextColor
                            font.pixelSize: parent.height / 2
                        }

                        MouseArea {
                            id: search_mouse_area
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                search_field.visible = !search_field.visible
                                search_field.clear()

                                if (search_field.visible) {
                                    search_field.forceActiveFocus()
                                }
                            }
                        }
                    }

                    TextField {
                        id: search_field
                        anchors.right: search_button.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        visible: false

                        onTextChanged: audio_tool.findElement(text)

                        Keys.onEscapePressed: {
                            if (search_field.activeFocus) {
                                search_field.clear()
                                search_field.visible = false
                            }
                        }
                    }
                }
            }

            // Info
            Item {
                id: audio_info_frame
                anchors.top: parent.top
                anchors.bottom: audio_control_bar.top
                anchors.right: parent.right
                anchors.rightMargin: 5
                width: platform.isAndroid ? parent.width / 5 : 175
                visible: true

                // Meta Data
                MetaDataDisplay {
                    id: meta_data_display
                    anchors.topMargin: 15
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                Text {
                    id: playlist_menu_text
                    anchors.top: meta_data_display.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 15

                    text: qsTr("Playlist")
                    wrapMode: Text.WordWrap
                    color: color_scheme.textColor
                    font.bold: true
                    visible: playlist_view.count > 0
                }

                // Playlist
                ListView {
                    id: playlist_view
                    anchors.top: playlist_menu_text.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 5

                    //                        height: parent.height - y - parent.padding * 2
                    //                        anchors.bottom:
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
