import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: root

    width: 300
    height: 200

    property alias text: edit.text
    property alias placeholderText: edit.placeholderText
    property alias font: edit.font
    property alias readOnly: edit.readOnly

    Rectangle {
        anchors.fill: flick
        color: palette.dark
        border.color: root.enabled ? palette.button : palette.dark
    }

    Flickable {
        id: flick

        anchors.fill: parent
        contentWidth: -1
        contentHeight: edit.paintedHeight
        clip: true

        ScrollBar.vertical: CustomScrollBar {
            id: scroll_bar
            visible: flick.contentHeight > flick.height
        }

        function ensureVisible(r) {
            if (contentX >= r.x)
                contentX = r.x
            else if (contentX + width <= r.x + r.width)
                contentX = r.x + r.width - width
            if (contentY >= r.y)
                contentY = r.y
            else if (contentY + height <= r.y + r.height)
                contentY = r.y + r.height - height
        }

        TextArea {
            id: edit
            width: scroll_bar.visible ? root.width - scroll_bar.width : root.width
            height: implicitHeight < flick.height ? flick.height : implicitHeight

            focus: true
            wrapMode: TextEdit.Wrap
            selectByMouse: true
            padding: 5

            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

            background: Item {}
        }
    }
}
