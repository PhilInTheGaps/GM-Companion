import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0

Rectangle {
    id: root
    color: palette.dark

    Flickable {
        id: flickable

        anchors.fill: parent
        anchors.margins: 5

        clip: true
        contentWidth: -1
        contentHeight: active_column.implicitHeight

        ScrollBar.vertical: ScrollBar {
            id: scroll_bar
            visible: flickable.contentHeight > flickable.height
        }

        Column {
            id: active_column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: scroll_bar.visible ? scroll_bar.width : 0

            Repeater {
                model: character_tool.characters

                CustomButton {
                    buttonText: modelData
                    anchors.left: parent.left
                    anchors.right: parent.right

                    textItem.width: mainRow.width - textItem.x
                                    - (icon_button.visible ? icon_button.width : 0)

                    onClicked: character_tool.setCurrentCharacter(index)

                    CustomToolBarButton {
                        id: icon_button
                        iconText: character_tool.active ? FontAwesome.times : FontAwesome.plus
                        anchors.right: parent.right
                        visible: parent.hovered || hovered

                        ToolTip.text: parent.active ? qsTr("Make Inactive") : qsTr(
                                                          "Make Active")
                        ToolTip.visible: hovered

                        onClicked: character_tool.toggleCharacterActive(index)
                    }
                }
            }
        }
    }
}
