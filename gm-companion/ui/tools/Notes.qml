import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3

import gm.companion.notestool 1.0
import gm.companion.colorscheme 1.0
import "../fontawesome"

Page {
    id: notes_page

    Component.onCompleted: notes_tool.loadChapters()

    NotesTool {
        id: notes_tool

        function setChapter(chapter) {
            setCurrentChapter(chapter)
        }

        function setPage(page) {
            setCurrentPage(page)
        }

        function loadChapters() {
            header_combo_box.model = chapters
        }

        onChaptersChanged: {
            loadChapters()
        }

        onCurrentChapterChanged: {
            updatePages()

            if (pages.length > 0) {
                setCurrentPage(pages[0])
            } else {
                text_edit.text = qsTr("No Pages added yet!")
            }

            header_combo_box.currentIndex = getCurrentChapterIndex()
        }

        onPagesChanged: {
            tab_button_repeater.model = 0

            for (var i = 0; i < pages.length; i++) {
                tab_button_repeater.model++
            }

            if (pages.length === 0)
                text_edit.readOnly = true
            else
                text_edit.readOnly = false

            header_tab_bar.setCurrentIndex(getCurrentPageIndex())
        }

        onCurrentPageChanged: {
            text_edit.text = currentPageContent
        }
    }

    ColorScheme {
        id: color_scheme
    }

    background: Rectangle {
        color: color_scheme.backgroundColor
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
        height: color_scheme.toolbarHeight
        color: color_scheme.toolbarColor

        Row {
            anchors.fill: parent
            spacing: 15

            Row {
                id: chapter_prop_row
                spacing: 15
                height: parent.height

                ComboBox {
                    id: header_combo_box
                    height: parent.height
                    width: 200
                    model: notes_tool.chapters

                    background: Rectangle {
                        color: "transparent"
                    }

                    contentItem: Text {
                        text: parent.displayText
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 5
                        rightPadding: parent.indicator.width - 5

                        elide: Text.ElideRight
                        color: color_scheme.toolbarTextColor
                        font.pointSize: 11
                    }

                    indicator: Icon {
                        icon: icons.fas_sort
                        pointSize: 15
                        anchors.verticalCenter: parent.verticalCenter
                        color: color_scheme.toolbarTextColor
                        x: parent.width - width - 5
                    }

                    onCurrentTextChanged: {
                        notes_tool.setChapter(currentText)
                    }
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

                    Icon {
                        id: delete_chapter_icon
                        icon: icons.fas_trash_alt
                        pointSize: 15
                        color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
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
                color: color_scheme.dividerColor
            }

            Button {
                id: add_button
                height: parent.height - parent.padding * 2
                width: add_icon.width
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    id: add_icon
                    icon: icons.fas_plus
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
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

                            color: header_tab_bar.currentIndex
                                   == index ? color_scheme.textColor : color_scheme.toolbarTextColor
                        }

                        height: header_tab_bar.height
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                        width: tab_button_text.width + 10

                        background: Rectangle {
                            color: header_tab_bar.currentIndex == index ? color_scheme.backgroundColor : color_scheme.toolbarColor
                        }
                    }
                }

                onCurrentIndexChanged: {
                    notes_tool.setCurrentPage(notes_tool.pages[currentIndex])
                }
            }
        }
    }

    ScrollView {
        anchors.left: parent.left
        anchors.right: right_rect.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        clip: true

        TextArea {
            id: text_edit
            selectByMouse: true
            wrapMode: TextEdit.WordWrap
            color: color_scheme.textColor
            font.pointSize: 12
            padding: 10

            onTextChanged: notes_tool.saveCurrentPageContent(text)
        }
    }

    Rectangle {
        id: right_rect
        width: color_scheme.toolbarHeight
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        color: color_scheme.toolbarColor

        Column {
            id: right_column
            anchors.fill: parent
            spacing: 10
            padding: 5
            topPadding: 10
            bottomPadding: 10

            DelayButton {
                id: delete_page_button
                width: parent.width - parent.padding * 2
                height: delete_page_icon.height + 10

                hoverEnabled: true
                delay: 600

                ToolTip.text: qsTr("Hold to delete page")
                ToolTip.visible: hovered

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    id: delete_page_icon
                    icon: icons.fas_trash_alt
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onActivated: notes_tool.deletePage(notes_tool.currentPage)
            }

            Button {
                id: encrypt_button
                width: parent.width - parent.padding * 2
                height: delete_page_icon.height + 10
                hoverEnabled: true

                ToolTip.text: qsTr("Encrypt Page")
                ToolTip.visible: hovered

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    id: encrypt_page_icon
                    icon: icons.fas_eye
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onClicked: text_edit.text = notes_tool.encrypt(text_edit.text)
            }

            Button {
                width: parent.width - parent.padding * 2
                height: delete_page_icon.height + 10
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    id: larger_font_icon
                    icon: icons.fas_search_plus
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onClicked: text_edit.font.pointSize++
            }

            Button {
                width: parent.width - parent.padding * 2
                height: delete_page_icon.height + 10
                hoverEnabled: true

                background: Rectangle {
                    color: "transparent"
                }

                Icon {
                    id: smaller_font_icon
                    icon: icons.fas_search_minus
                    pointSize: 15
                    color: parent.pressed ? "grey" : parent.hovered ? "lightgrey" : color_scheme.toolbarTextColor
                    anchors.centerIn: parent
                }

                onClicked: text_edit.font.pointSize--
            }
        }
    }
}
