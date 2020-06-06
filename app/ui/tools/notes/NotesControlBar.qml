import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines

Item {
    id: root

    property var textedit: undefined

    height: Defines.TOOLBAR_HEIGHT

    Row {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 5
        spacing: 10

        Row {
            id: doc_controls
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            CustomToolBarButton {
                iconText: notes_tool.editMode ? FontAwesome.eye : FontAwesome.edit

                onClicked: notes_tool.editMode = !notes_tool.editMode
            }

            CustomToolBarButton {
                iconText: FontAwesome.save

                onClicked: {
                    if (notes_tool.currentPage) {
                        notes_tool.currentPage.save()
                    }
                }
            }

            CustomToolBarButton {
                iconText: FontAwesome.lock
                toolTipText: qsTr("En-/Decrypt using ROT13")
                onClicked: notes_tool.encrypt()
            }

            CustomToolBarButton {
                iconText: FontAwesome.filePdf
                toolTipText: qsTr("Export as PDF")
                visible: !notes_tool.editMode
                onClicked: notes_tool.exportPdf()
            }
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height / 2
            width: 2
            color: palette.dark
            visible: notes_tool.editMode
        }

        Row {
            id: md_controls
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            visible: notes_tool.editMode

            CustomToolBarButton {
                iconText: FontAwesome.heading
                onClicked: header_dialog.open()
            }

            CustomToolBarButton {
                iconText: FontAwesome.bold
                onClicked: insertMarkdown("**", "**")
            }

            CustomToolBarButton {
                iconText: FontAwesome.italic
                onClicked: insertMarkdown("*", "*")
            }

            CustomToolBarButton {
                iconText: FontAwesome.strikethrough
                onClicked: insertMarkdown("~~", "~~")
            }

            CustomToolBarButton {
                iconText: FontAwesome.quoteLeft
                onClicked: insertMarkdown("> ", "")
            }

            CustomToolBarButton {
                iconText: FontAwesome.code
                onClicked: insertMarkdown("`", "`")
            }

            CustomToolBarButton {
                iconText: FontAwesome.listUl
                onClicked: insertMarkdown("* ", "")
            }

            CustomToolBarButton {
                iconText: FontAwesome.listOl
                onClicked: insertMarkdown("1. ", "")
            }

            CustomToolBarButton {
                iconText: FontAwesome.table
                onClicked: {
                    insertMarkdown("| aaa | bbb |\n", "")
                    insertMarkdown("| --- | --- |\n", "")
                    insertMarkdown("| ccc | ddd |\n", "")
                    insertMarkdown("| eee | fff |\n", "")
                }
            }
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height / 2
            width: 2
            color: palette.dark
            visible: notes_tool.editMode
        }

        Row {
            id: opened_pages_row
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Repeater {
                model: notes_tool.openedPages

                CustomButton {
                    id: page_button
                    buttonText: modelData.name
                    usesFixedWidth: false
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom

                    textItem.font.italic: !modelData.wasEdited
                    enableBold: false
                    transparentBackground: !modelData.isCurrent

                    leftPadding: 10
                    rightPadding: close_page_button.width + 5

                    CustomToolBarButton {
                        id: close_page_button
                        anchors.right: parent.right
                        visible: page_button.mouseArea.containsMouse
                                 || mouseArea.containsMouse
                                 || !modelData.isSaved || modelData.isCurrent

                        iconText: mouseArea.containsMouse ? FontAwesome.timesCircle : (modelData.isSaved ? FontAwesome.times : FontAwesome.asterisk)
                        pointSize: 10

                        onClicked: {
                            if (modelData.isSaved) {
                                modelData.close()
                            } else {
                                save_dialog.page = modelData
                                save_dialog.open()
                            }
                        }
                    }

                    onClicked: modelData.toggle()
                }
            }
        }
    }

    Rectangle {
        color: palette.dark
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 1
    }

    InsertHeaderDialog {
        id: header_dialog

        onInsert: insertMarkdown(text, "")

        y: parent.height
        x: md_controls.x + 5
    }

    SavePageDialog {
        id: save_dialog

        y: parent.height
        x: (root.width - width) / 2
    }

    function insertMarkdown(open, close) {
        if (!root.textedit || !notes_tool.currentPage || !notes_tool.editMode)
            return

        if (root.textedit.selectedText.length) {
            root.textedit.insert(root.textedit.selectionStart, open)
            root.textedit.insert(root.textedit.selectionEnd, close)
            root.textedit.select(root.textedit.selectionStart,
                                 root.textedit.selectionEnd - close.length)
        } else {
            root.textedit.insert(root.textedit.cursorPosition, open + close)

            if (close.length) {
                moveCursor(close.length * -1)
            }
        }
    }

    function moveCursor(positions) {
        root.textedit.cursorPosition += positions
    }
}
