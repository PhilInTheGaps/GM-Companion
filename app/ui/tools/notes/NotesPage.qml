import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Item {
    id: root

    property alias textedit: textedit.edit

    CustomTextEdit {
        id: textedit

        property bool justChangedCursor: false
        property int tempCursorPosition: 0

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        background.color: palette.base
        background.border.width: 0
        font.pointSize: 12

        edit.readOnly: !notes_tool.currentPage || !notes_tool.editMode

        Component.onCompleted: {
            notes_tool.qmlTextDoc = textDocument
        }

        onCursorPositionChanged: {
            // Sometimes the cursor position changes again
            // after it was manually set, so here is
            // an ugly workaround for that.
            if (position !== tempCursorPosition
                    && position !== textedit.edit.length) {
                justChangedCursor = false

                if (notes_tool.currentPage) {
                    notes_tool.currentPage.cursorPosition = position
                }
            } else if (justChangedCursor) {
                textedit.edit.cursorPosition = tempCursorPosition
            }
        }

        onLinkClicked: {
            notes_tool.linkClicked(link)
        }

        Connections {
            target: main_window
            onSave: {
                if (notes_tool.currentPage) {
                    if (notes_tool.editMode) {
                        // Trim whitespace
                        if (textedit.edit.text.endsWith(" ")
                                || textedit.edit.text.startsWith(" ")) {
                            textedit.text = textedit.text.trim()
                        }

                        // Ensure newline
                        if (!textedit.text.endsWith("\n")) {
                            textedit.edit.append("\n")
                        }
                    }

                    notes_tool.currentPage.save()
                }
            }
        }

        Connections {
            target: notes_tool
            onSetCursorPosition: {
                textedit.justChangedCursor = true
                textedit.tempCursorPosition = position
                textedit.edit.cursorPosition = position
            }
        }
    }
}
