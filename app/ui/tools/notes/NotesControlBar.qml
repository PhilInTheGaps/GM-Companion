pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents
import IconFonts
import src
import "../.."

Item {
    id: root

    property TextArea textArea

    height: Sizes.toolbarHeight

    function insertMarkdown(open: string, close: string) {
        if (!root.textArea || !NotesTool.currentPage || !NotesTool.editMode)
            return

        if (root.textArea.selectedText.length) {
            root.textArea.insert(root.textArea.selectionStart, open)
            root.textArea.insert(root.textArea.selectionEnd, close)
            root.textArea.select(root.textArea.selectionStart,
                                 root.textArea.selectionEnd - close.length)
        } else {
            root.textArea.insert(root.textArea.cursorPosition, open + close)

            if (close.length) {
                moveCursor(close.length * -1)
            }
        }
    }

    function moveCursor(positions: int) {
        root.textArea.cursorPosition += positions
    }

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
                iconText: NotesTool && NotesTool.editMode ? FontAwesome.eye : FontAwesome.penToSquare

                onClicked: NotesTool.editMode = !NotesTool.editMode
            }

            CustomToolBarButton {
                iconText: FontAwesome.floppyDisk

                onClicked: {
                    if (NotesTool.currentPage) {
                        NotesTool.currentPage.save()
                    }
                }
            }

            CustomToolBarButton {
                iconText: FontAwesome.lock
                toolTipText: qsTr("En-/Decrypt using ROT13.\nNote: This is not secure encryption and only intended to prevent others from accidentally reading your notes.")
                onClicked: NotesTool.encrypt()
            }

            CustomToolBarButton {
                iconText: FontAwesome.filePdf
                toolTipText: qsTr("Export as PDF")
                visible: !NotesTool.editMode
                onClicked: NotesTool.exportPdf()
            }
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height / 2
            width: 2
            color: palette.dark
            visible: NotesTool.editMode
        }

        Row {
            id: md_controls
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            visible: NotesTool.editMode

            CustomToolBarButton {
                iconText: FontAwesome.heading
                onClicked: header_dialog.open()
            }

            CustomToolBarButton {
                iconText: FontAwesome.bold
                onClicked: root.insertMarkdown("**", "**")
            }

            CustomToolBarButton {
                iconText: FontAwesome.italic
                onClicked: root.insertMarkdown("*", "*")
            }

            CustomToolBarButton {
                iconText: FontAwesome.strikethrough
                onClicked: root.insertMarkdown("~~", "~~")
            }

            CustomToolBarButton {
                iconText: FontAwesome.quoteLeft
                onClicked: root.insertMarkdown("> ", "")
            }

            CustomToolBarButton {
                iconText: FontAwesome.code
                onClicked: root.insertMarkdown("`", "`")
            }

            CustomToolBarButton {
                iconText: FontAwesome.listUl
                onClicked: root.insertMarkdown("* ", "")
            }

            CustomToolBarButton {
                iconText: FontAwesome.listOl
                onClicked: root.insertMarkdown("1. ", "")
            }

            CustomToolBarButton {
                iconText: FontAwesome.table
                onClicked: {
                    root.insertMarkdown("| aaa | bbb |\n", "")
                    root.insertMarkdown("| --- | --- |\n", "")
                    root.insertMarkdown("| ccc | ddd |\n", "")
                    root.insertMarkdown("| eee | fff |\n", "")
                }
            }
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height / 2
            width: 2
            color: palette.dark
            visible: NotesTool.editMode
        }

        Row {
            id: opened_pages_row
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Repeater {
                model: NotesTool.openedPages

                CustomButton {
                    id: page_button

                    required property NoteBookPage modelData

                    buttonText: modelData.name // qmllint disable missing-property
                    usesFixedWidth: false
                    anchors.top: parent ? parent.top : undefined
                    anchors.bottom: parent ? parent.bottom : undefined

                    textItem.font.italic: !modelData.keepOpen
                    enableBold: false
                    transparentBackground: !modelData.isCurrent

                    leftPadding: 10
                    rightPadding: close_page_button.width + 5

                    CustomToolBarButton {
                        id: close_page_button
                        anchors.right: parent.right
                        visible: page_button.mouseArea.containsMouse
                                 || page_button.mouseArea.containsMouse
                                 || !page_button.modelData.isSaved || page_button.modelData.isCurrent

                        iconText: page_button.mouseArea.containsMouse || mouseArea.containsMouse ? FontAwesome.circleXmark : (page_button.modelData.isSaved ? FontAwesome.xmark : FontAwesome.asterisk)
                        pointSize: 10

                        onClicked: {
                            if (page_button.modelData.isSaved) {
                                page_button.modelData.close()
                            } else {
                                save_dialog.page = page_button.modelData
                                save_dialog.open()
                            }
                        }
                    }

                    onClicked: modelData.toggle() // qmllint disable missing-property
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

        onInsert: (text) => root.insertMarkdown(text, "")

        y: parent.height
        x: md_controls.x + 5
    }

    SavePageDialog {
        id: save_dialog

        y: parent.height
        x: (root.width - width) / 2
    }
}
