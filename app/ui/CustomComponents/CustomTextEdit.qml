import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    width: 300
    height: 200

    property alias area: area
    property alias text: area.text
    property alias placeholderText: area.placeholderText
    property alias font: area.font
    property alias readOnly: area.readOnly
    property alias textDocument: area.textDocument

    signal cursorPositionChanged(int position)
    signal linkClicked(string link)

    color: palette.dark
    border.color: root.enabled ? palette.button : palette.dark

    ScrollView {
        id: scrollview

        anchors.fill: parent
        clip: true

        TextArea {
            id: area

            focus: true
            wrapMode: TextEdit.Wrap
            selectByMouse: true
            hoverEnabled: true
            padding: 5

            onCursorPositionChanged: root.cursorPositionChanged(cursorPosition)
            onLinkActivated: link => root.linkClicked(link)

            background: Item {}

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.NoButton
                cursorShape: area.readOnly ? (area.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor) : Qt.IBeamCursor
            }
        }
    }
}
