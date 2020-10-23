import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../colors.js" as Colors
import "../../sizes.js" as Sizes

ListView {
    id: list_view

    clip: true
    spacing: 10

    model: combatantListModel
    currentIndex: combat_tracker.currentIndex

    ScrollBar.vertical: ScrollBar {
        id: scroll_bar
        visible: list_view.contentHeight > list_view.height
    }

    delegate: Rectangle {
        id: delegate_root
        height: delegate_row.height
        anchors.left: parent ? parent.left : undefined
        anchors.right: parent ? parent.right : undefined
        anchors.margins: 5
        anchors.rightMargin: scroll_bar.visible ? scroll_bar.width + 5 : 5
        color: ListView.isCurrentItem ? Colors.dark : "transparent"
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
                    text: modelData.name
                    color: delegate_root.ListView.isCurrentItem ? Colors.buttonText : Colors.text
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
                        visible: modelData ? modelData.delay : false

                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter

                        color: Colors.text
                        opacity: 0.7

                        text: FontAwesome.clock
                        font.family: FontAwesome.familySolid
                    }
                }
            }

            // INI
            ListSpinBox {
                value: modelData.ini
                width: list_view.width / 6
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                font_color: delegate_root.ListView.isCurrentItem ? Colors.buttonText : Colors.text
                onValueEdited: combat_tracker.setIni(index, new_value)
                onValueIncreased: combat_tracker.modifyIni(index, steps)
            }

            // Health
            ListSpinBox {
                width: list_view.width / 6
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                value: modelData.health
                font_color: delegate_root.ListView.isCurrentItem ? Colors.buttonText : Colors.text
                onValueEdited: combat_tracker.setHealth(index, new_value)
                onValueIncreased: combat_tracker.modifyHealth(index, steps)
            }

            // Priority
            ListSpinBox {
                width: list_view.width / 6
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                value: modelData.priority
                font_color: delegate_root.ListView.isCurrentItem ? Colors.buttonText : Colors.text
                onValueEdited: combat_tracker.setPriority(index, new_value)
                onValueIncreased: combat_tracker.modifyPriority(index, steps)
            }

            // Notes
            ListTextField {
                width: parent.width - x - parent.padding - parent.spacing
                       - delegate_button_row.width
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
                    toolTipText: qsTr("Delay Turn")
                }

                // Remove Character
                CustomToolBarButton {
                    id: deletgate_remove_button
                    iconText: FontAwesome.times
                    toolTipText: qsTr("Remove from tracker")
                    onClicked: combat_tracker.remove(index)
                }
            }
        }
    }
}
