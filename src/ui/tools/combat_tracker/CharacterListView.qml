import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines

ListView {
    id: list_view

    clip: true
    spacing: 10

    model: combatantListModel
    currentIndex: combat_tracker.currentIndex

    ScrollBar.vertical: CustomScrollBar {
        visible: list_view.contentHeight > list_view.height
    }

    delegate: Rectangle {
        id: delegate_root
        height: delegate_row.height
        anchors.left: parent.left
        anchors.right: parent.right
        color: ListView.isCurrentItem ? palette.button : "transparent"

        Row {
            id: delegate_row
            padding: 10
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            height: Defines.TOOLBAR_HEIGHT

            // Name
            Label {
                text: modelData.name
                color: delegate_root.ListView.isCurrentItem ? palette.buttonText : palette.text
                width: list_view.width / 5
                clip: true
                elide: Text.ElideRight
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 12
                font.bold: true
            }

            // INI
            ListSpinBox {
                value: modelData.ini
                width: list_view.width / 6
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                font_color: delegate_root.ListView.isCurrentItem ? palette.buttonText : palette.text
                onValueEdited: combat_tracker.setIni(index, new_value)
                onValueIncreased: combat_tracker.modifyIni(index, steps)
            }

            // Health
            ListSpinBox {
                width: list_view.width / 6
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                value: modelData.health
                font_color: delegate_root.ListView.isCurrentItem ? palette.buttonText : palette.text
                onValueEdited: combat_tracker.setHealth(index, new_value)
                onValueIncreased: combat_tracker.modifyHealth(index, steps)
            }

            // Notes
            ListTextField {
                width: parent.width - x - 20 - delegate_button_row.width
                onFieldTextChanged: combat_tracker.setNotes(index, fieldText)
                text: modelData.notes
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
                    onClicked: combat_tracker.delayTurn(index)
                }

                // Remove Character
                CustomToolBarButton {
                    id: deletgate_remove_button
                    iconText: FontAwesome.times
                    onClicked: combat_tracker.remove(index)
                }
            }
        }
    }
}
