pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "./buttons"
import "../.."

Item {
    id: audio_scenario_element_column

    // Scenarios
    Flow {
        id: scenario_flow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Repeater {
            id: scenario_repeater

            model: AudioTool.currentProject && AudioTool.currentProject.currentCategory ? AudioTool.currentProject.currentCategory.scenarios : []

            CustomButton {
                required property AudioScenario modelData

                buttonText: modelData.name // qmllint disable missing-property
                padding: 10
                backgroundColor: "transparent"
                usesFixedWidth: false
                onClicked: {
                    var project = AudioTool.currentProject;
                    if (project && project.currentCategory) {
                        project.currentCategory.currentScenario = modelData;
                    }
                }
            }
        }
    }

    // Elements
    ListView {
        id: list

        readonly property int min_width: 170
        readonly property int count_per_row: parent ? Math.floor(width / min_width) : 1
        readonly property int button_width: (width - verticalScrollBar.width) / count_per_row

        anchors.top: scenario_flow.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        spacing: 10

        clip: true
        model: (AudioTool.currentProject && AudioTool.currentProject.currentScenario) ? AudioTool.currentProject.currentScenario.model : []

        ScrollBar.vertical: ScrollBar {
            id: verticalScrollBar
            anchors.top: list.top
            anchors.right: list.right
            anchors.bottom: list.bottom
            visible: list.contentHeight > list.height

            interactive: list.model.length < 2
        }

        delegate: Column {
            id: list_delegate

            required property AudioScenario modelData
            required property int index

            spacing: 5
            anchors.left: parent ? parent.left : undefined
            anchors.right: parent ? parent.right : undefined

            Label {
                id: subscenario_text
                text: list_delegate.modelData.name // qmllint disable missing-property
                font.pointSize: 12
                verticalAlignment: Text.AlignVCenter
                visible: list_delegate.index > 0

                anchors.right: parent.right
                anchors.left: parent.left
                elide: Text.ElideRight
            }

            Flow {
                anchors.left: parent.left
                anchors.right: parent.right

                Repeater {
                    model: list_delegate.modelData.elements

                    Item {
                        id: flow_delegate

                        required property AudioElement modelData

                        width: list.button_width
                        height: width

                        AudioButton {
                            id: element_button

                            element_name: flow_delegate.modelData.name // qmllint disable missing-property
                            element_type: flow_delegate.modelData.type
                            subscenario_name: subscenario_text.text
                            thumbnail: flow_delegate.modelData.thumbnail
                            width: parent.width - 4
                            anchors.centerIn: parent

                            onClicked: AudioTool.play(flow_delegate.modelData)
                        }
                    }
                }
            }
        }
    }

    // Search
    Item {
        height: Sizes.toolbarHeight
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 10

        // Search Button
        Rectangle {
            id: search_button
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: height
            radius: search_field.visible ? 0 : width / 2

            color: palette.dark
            border.color: palette.button
            border.width: 1

            Label {
                font.family: FontAwesome.fontSolid.family
                font.styleName: FontAwesome.fontSolid.styleName
                text: search_field.visible ? FontAwesome.xmark : FontAwesome.magnifyingGlass
                anchors.centerIn: parent
                opacity: search_mouse_area.pressed ? 0.5 : search_mouse_area.containsMouse ? 0.75 : 1
                color: palette.buttonText
                font.pixelSize: parent.height / 2
            }

            MouseArea {
                id: search_mouse_area
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    search_field.visible = !search_field.visible;
                    search_field.clear();
                    if (search_field.visible) {
                        search_field.forceActiveFocus();
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

            placeholderText: qsTr("Search ...")

            scale: visible ? 1.0 : 0.1
            transformOrigin: Item.Right

            Behavior on scale {
                NumberAnimation {
                    duration: 100
                    easing.type: Easing.Linear
                }
            }

            onTextChanged: AudioTool.findElement(text)

            Keys.onEscapePressed: {
                if (search_field.activeFocus) {
                    search_field.clear();
                    search_field.visible = false;
                }
            }
        }
    }
}
