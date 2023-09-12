import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src

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
                model: CharacterTool.characters

                CustomButton {
                    id: character_delegate
                    required property string modelData
                    required property int index

                    buttonText: modelData
                    anchors.left: parent.left
                    anchors.right: parent.right

                    textItem.width: mainRow.width - textItem.x
                                    - (icon_button.visible ? icon_button.width : 0)

                    onClicked: CharacterTool.setCurrentCharacter(index)

                    CustomToolBarButton {
                        id: icon_button
                        iconText: CharacterTool.active ? FontAwesome.xmark : FontAwesome.plus
                        anchors.right: parent.right
                        visible: parent.hovered || hovered

                        ToolTip.text: CharacterTool.active ? qsTr("Make Inactive") : qsTr(
                                                                  "Make Active")
                        ToolTip.visible: hovered

                        onClicked: CharacterTool.toggleCharacterActive(character_delegate.index)
                    }
                }
            }
        }
    }
}
