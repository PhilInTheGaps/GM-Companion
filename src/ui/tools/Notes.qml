import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../defines.js" as Defines

Page {
    id: notes_page

    Component.onCompleted: loadChapters()

    function setChapter(chapter) {
        notes_tool.setCurrentChapter(chapter)
    }

    function setPage(page) {
        notes_tool.setCurrentPage(page)
    }

    function loadChapters() {
        header_combo_box.model = notes_tool.chapters
    }

    Connections {
        target: notes_tool

        onChaptersChanged: {
            loadChapters()
        }

        onCurrentChapterChanged: {
            notes_tool.updatePages()

            if (notes_tool.pages.length > 0) {
                notes_tool.setCurrentPage(notes_tool.pages[0])
            } else {
                text_edit.text = qsTr("No Pages added yet!")
            }

            header_combo_box.currentIndex = notes_tool.getCurrentChapterIndex()
        }

        onPagesChanged: {
            tab_button_repeater.model = 0

            for (var i = 0; i < notes_tool.pages.length; i++) {
                tab_button_repeater.model++
            }

            if (notes_tool.pages.length === 0)
                text_edit.readOnly = true
            else
                text_edit.readOnly = false

            header_tab_bar.setCurrentIndex(notes_tool.getCurrentPageIndex())
        }

        onCurrentPageChanged: {
            text_edit.text = notes_tool.currentPageContent
        }
    }

    header: header_rect

    Dialog {
        id: add_dialog
        x: add_button.x + add_button.width + 10
        y: 10
        title: qsTr("Add Page / Chapter")

        contentItem: Column {
            padding: 10
            spacing: 10

            ComboBox {
                id: type_combo_box
                model: [qsTr("Page"), qsTr("Chapter")]
            }

            TextField {
                id: title_field
                placeholderText: qsTr("Title")
                width: parent.width - parent.padding * 2
                enabled: type_combo_box.currentIndex
                         == 0 ? (category_combo_box.currentIndex > -1 ? true : false) : true
            }

            ComboBox {
                id: category_combo_box
                enabled: type_combo_box.currentIndex == 0
                model: notes_tool.chapters
            }
        }

        standardButtons: Dialog.Ok | Dialog.Cancel

        onOpened: {
            type_combo_box.currentIndex = 0
            title_field.clear()
        }

        onAccepted: {
            if (title_field.text != "") {
                switch (type_combo_box.currentIndex) {
                case 0:
                    if (category_combo_box.currentIndex > -1)
                        notes_tool.addPage(title_field.text)
                    break
                case 1:
                    notes_tool.addChapter(title_field.text)
                    break
                default:
                    console.error("Error: No Type Selected!")
                }
            }
        }
    }

    Rectangle {
        id: header_rect
        height: Defines.TOOLBAR_HEIGHT
        color: palette.alternateBase

        Row {
            anchors.fill: parent
            spacing: 15

            Row {
                id: chapter_prop_row
                spacing: 15
                height: parent.height

                CustomToolBarComboBox {
                    id: header_combo_box
                    width: 200
                    model: notes_tool.chapters

                    onCurrentTextChanged: setChapter(currentText)
                }

                DelayButton {
                    id: delete_chapter_button
                    height: parent.height - parent.padding * 2
                    width: delete_chapter_icon.width
                    hoverEnabled: true
                    delay: 600

                    ToolTip.text: qsTr("Hold to delete chapter")
                    ToolTip.visible: hovered

                    background: Rectangle {
                        color: "transparent"
                    }

                    Text {
                        id: delete_chapter_icon
                        text: FontAwesome.trashAlt
                        font.family: FontAwesome.familySolid
                        font.pointSize: 15
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : palette.text
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    onActivated: {
                        if (header_combo_box.currentText != "")
                            notes_tool.deleteChapter(
                                        header_combo_box.currentText)

                        progress = 0
                    }
                }
            }

            Rectangle {
                id: header_spacer_1
                width: 1
                height: parent.height - parent.padding * 2
                color: palette.button
            }

            Button {
                id: add_button
                height: parent.height - parent.padding * 2
                width: add_icon.width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Text {
                    id: add_icon
                    text: FontAwesome.plus
                    font.family: FontAwesome.familySolid
                    font.pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : palette.text
                    anchors.verticalCenter: parent.verticalCenter
                }

                onClicked: {
                    add_dialog.open()
                }
            }

            TabBar {
                id: header_tab_bar
                height: parent.height - parent.padding * 2
                width: parent.width - x - parent.rightPadding

                clip: true

                background: Rectangle {
                    color: "transparent"
                }

                Repeater {
                    id: tab_button_repeater
                    model: 0

                    TabButton {
                        Text {
                            id: tab_button_text
                            text: notes_tool.pages[index]
                            anchors.centerIn: parent
                            font.pointSize: 11

                            color: palette.text
                        }

                        height: header_tab_bar.height
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                        width: tab_button_text.width + 10

                        background: Rectangle {
                            color: header_tab_bar.currentIndex
                                   == index ? palette.base : palette.alternateBase
                        }
                    }
                }

                onCurrentIndexChanged: {
                    notes_tool.setCurrentPage(notes_tool.pages[currentIndex])
                }
            }
        }
    }

    CustomTextEdit {
        id: text_edit
        anchors.left: parent.left
        anchors.right: right_rect.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        //onTextChanged: notes_tool.saveCurrentPageContent(text)
    }

    Rectangle {
        id: right_rect
        width: Defines.TOOLBAR_WIDTH
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        color: palette.alternateBase

        Column {
            id: right_column
            anchors.fill: parent
            spacing: 5
            topPadding: 5

            CustomToolBarButton {
                iconText: FontAwesome.trashAlt
                verticalMode: true
                outline: true
                height: width

                ToolTip.text: qsTr("Delete page")
                ToolTip.visible: hovered

                onClicked: notes_tool.deletePage(notes_tool.currentPage)
                // TODO: Confirm
            }

            CustomToolBarButton {
                iconText: FontAwesome.eye
                verticalMode: true
                outline: true
                height: width

                ToolTip.text: qsTr("Encrypt page (using ROT13)")
                ToolTip.visible: hovered

                onClicked: text_edit.text = notes_tool.encrypt(text_edit.text)
            }

            CustomToolBarButton {
                iconText: FontAwesome.searchPlus
                verticalMode: true
                outline: true
                height: width

                onClicked: text_edit.font.pointSize++
            }

            CustomToolBarButton {
                iconText: FontAwesome.searchMinus
                verticalMode: true
                outline: true
                height: width

                onClicked: text_edit.font.pointSize--
            }
        }
    }
}
