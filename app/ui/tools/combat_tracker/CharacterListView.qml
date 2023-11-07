pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."

ListView {
    id: list_view

    clip: true
    spacing: 10

    model: CombatTrackerTool.model
    currentIndex: CombatTrackerTool.currentIndex

    ScrollBar.vertical: ScrollBar {
        id: scroll_bar
        visible: list_view.contentHeight > list_view.height
    }

    delegate: Rectangle {
        id: delegate_root

        required property Combatant modelData
        required property int index

        height: delegate_row.height
        anchors.left: parent ? parent.left : undefined
        anchors.right: parent ? parent.right : undefined
        anchors.margins: 5
        anchors.rightMargin: scroll_bar.visible ? scroll_bar.width + 5 : 5
        color: ListView.isCurrentItem ? palette.dark : "transparent"
        border.color: Colors.border
        border.width: ListView.isCurrentItem ? 1 : 0

        Row {
            id: delegate_row
            padding: 10
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            height: Sizes.toolbarHeight

            // Name
            Item {
                width: list_view.width / 5
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Label {
                    text: delegate_root.modelData ? delegate_root.modelData.name : ""
                    color: delegate_root.ListView.isCurrentItem ? palette.buttonText : palette.text
                    width: delay_indicator.visible ? parent.width - delay_indicator.width
                                                     - 5 : parent.width

                    clip: true
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 12
                    font.bold: true

                    // Delay indicator
                    Label {
                        id: delay_indicator
                        visible: delegate_root.modelData ? delegate_root.modelData.delay : false

                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        color: palette.text
                        opacity: 0.7

                        text: FontAwesome.clock
                        font.family: FontAwesome.fontSolid.family
                        font.styleName: FontAwesome.fontSolid.styleName
                    }
                }
            }

            // INI
            ListSpinBox {
                value: delegate_root.modelData ? delegate_root.modelData.ini : 0
                width: list_view.width / 6
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                font_color: delegate_root.ListView.isCurrentItem ? palette.buttonText : palette.text
                onValueEdited: new_value => CombatTrackerTool.setIni(delegate_root.index, new_value)
                onValueIncreased: steps => CombatTrackerTool.modifyIni(delegate_root.index, steps)
            }

            // Health
            ListSpinBox {
                width: list_view.width / 6
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                value: delegate_root.modelData ? delegate_root.modelData.health : 0
                font_color: delegate_root.ListView.isCurrentItem ? palette.buttonText : palette.text
                onValueEdited: new_value => CombatTrackerTool.setHealth(delegate_root.index,
                                                                     new_value)
                onValueIncreased: steps => CombatTrackerTool.modifyHealth(delegate_root.index, steps)
            }

            // Priority
            ListSpinBox {
                width: list_view.width / 6
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                value: delegate_root.modelData ? delegate_root.modelData.priority : 0
                font_color: delegate_root.ListView.isCurrentItem ? palette.buttonText : palette.text
                onValueEdited: new_value => CombatTrackerTool.setPriority(
                                   delegate_root.index, new_value)
                onValueIncreased: steps => CombatTrackerTool.modifyPriority(delegate_root.index, steps)
            }

            // Notes
            ListTextField {
                width: parent.width - x - parent.padding - parent.spacing
                       - delegate_button_row.width
                onFieldTextChanged: CombatTrackerTool.setNotes(delegate_root.index, fieldText)
                text: delegate_root.modelData ? delegate_root.modelData.notes : ""
            }

            // Buttons
            Row {
                id: delegate_button_row
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: parent.padding

                // Delay Turn
                CustomToolBarButton {
                    id: delegate_delay_button
                    iconText: FontAwesome.clock
                    visible: delegate_root.ListView.isCurrentItem
                    onClicked: CombatTrackerTool.delayTurn(delegate_root.index)
                    toolTipText: qsTr("Delay Turn")
                }

                // Remove Character
                CustomToolBarButton {
                    id: deletgate_remove_button
                    iconText: FontAwesome.xmark
                    toolTipText: qsTr("Remove from tracker")
                    onClicked: CombatTrackerTool.remove(delegate_root.index)
                }
            }
        }
    }
}
