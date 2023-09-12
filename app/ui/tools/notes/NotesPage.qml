import QtQuick
import CustomComponents
import src

Item {
    id: root

    property alias textArea: textedit.area

    function onSave() {
        if (NotesTool.currentPage) {
            if (NotesTool.editMode) {
                // Trim whitespace
                if (textArea.text.endsWith(" ")
                        || textArea.text.startsWith(" ")) {
                    textedit.text = textedit.text.trim()
                }

                // Ensure newline
                if (!textedit.text.endsWith("\n")) {
                    textArea.append("\n")
                }
            }

            NotesTool.currentPage.save()
        }
    }

    Shortcut {
        sequences: [StandardKey.Save]
        onActivated: root.onSave()
        context: Qt.ApplicationShortcut
    }

    Connections {
        target: NotesTool

        function onSetCursorPosition(position) {
            textedit.justChangedCursor = true
            textedit.tempCursorPosition = position
            root.textArea.cursorPosition = position
        }
    }

    CustomTextEdit {
        id: textedit

        property bool justChangedCursor: false
        property int tempCursorPosition: 0

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        color: palette.base
        border.width: 0
        font.pointSize: 12

        area.readOnly: !NotesTool.currentPage || !NotesTool.editMode

        Component.onCompleted: {
            NotesTool.qmlTextDoc = textDocument
        }

        onCursorPositionChanged: (position) => {
            // Sometimes the cursor position changes again
            // after it was manually set, so here is
            // an ugly workaround for that.
            if (position !== tempCursorPosition
                    && position !== textedit.area.length) {
                justChangedCursor = false

                if (NotesTool.currentPage) {
                    NotesTool.currentPage.cursorPosition = position
                }
            } else if (justChangedCursor) {
                textedit.area.cursorPosition = tempCursorPosition
            }
        }

        onLinkClicked: (link) => {
            NotesTool.linkClicked(link)
        }
    }
}
