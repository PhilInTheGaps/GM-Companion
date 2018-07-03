import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

import gm.companion.notestool 1.0
import gm.companion.colorscheme 1.0
import "./notes"

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
            chapter_column.children = []
            page_column.children = []

            var component = Qt.createComponent("./notes/ChapterButton.qml")

            for (var i = 0; i < chapters.length; i++) {
                var button = component.createObject(chapter_column, {
                                                        x: 0,
                                                        y: 0,
                                                        chapter: chapters[i],
                                                        parent_width: chapter_column.width
                                                    })
                button.clicked.connect(setChapter)
            }
        }

        onChaptersChanged: {
            loadChapters()
        }

        onCurrentChapterChanged: {
            updatePages()

            if (pages.length > 0) {
                setCurrentPage(pages[0])
            } else {
                text_edit.text = ""
            }
        }

        onPagesChanged: {
            page_column.children = []

            var component = Qt.createComponent("./notes/PageButton.qml")

            for (var i = 0; i < pages.length; i++) {
                var button = component.createObject(page_column, {
                                                        x: 0,
                                                        y: 0,
                                                        page: pages[i],
                                                        parent_width: page_column.width
                                                    })
                button.clicked.connect(setPage)
            }
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

    Row {
        anchors.fill: parent
        spacing: 5
        padding: 5

        Column {
            id: left_column
            height: parent.height - parent.topPadding * 2
            width: platform.isAndroid ? parent.width / 5 : parent.width / 8
            spacing: 5

            Text {
                id: font_size_text
                text: qsTr("Font Size:")
                color: color_scheme.textColor
            }

            SpinBox {
                id: font_size_spin_box
                value: 12
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

                //                onValueChanged: text_edit.font.pointSize = value
            }

            TextField {
                id: add_chapter_text_field
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40
                placeholderText: qsTr("Chapter")
            }

            Button {
                id: add_chapter_button
                text: qsTr("Add Chapter")
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

                onClicked: notes_tool.addChapter(add_chapter_text_field.text)
            }

            DelayButton {
                id: delete_chapter_button
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

                text: qsTr("Delete Chapter")
                delay: 1200
                onActivated: notes_tool.deleteChapter(notes_tool.currentChapter)
            }

            Frame {
                id: chapters_text_frame
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40
                padding: 5

                Text {
                    text: qsTr("Chapters")
                    color: color_scheme.textColor
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            ScrollView {
                width: parent.width
                height: parent.height - font_size_text.height
                        - font_size_spin_box.height - chapters_text_frame.height
                        - delete_chapter_button.height - add_chapter_text_field.height
                        - add_chapter_button.height - parent.spacing * 6

                clip: true

                Column {
                    id: chapter_column
                    width: parent.parent.width

                    spacing: 5
                }
            }
        }

        Column {
            id: text_edit_column
            height: parent.height - parent.topPadding * 2
            width: parent.width - left_column.width - parent.spacing * 2
                   - parent.leftPadding * 2 - right_column.width

            ScrollView {
                width: parent.width
                height: parent.height
                clip: true

                TextArea {
                    id: text_edit
                    width: parent.width

                    selectByMouse: true
                    wrapMode: TextEdit.WordWrap
                    color: color_scheme.textColor
                    font.pointSize: font_size_spin_box.value

                    onTextChanged: notes_tool.saveCurrentPageContent(text)
                }
            }
        }

        Column {
            id: right_column
            width: platform.isAndroid ? parent.width / 5 : parent.width / 8
            height: parent.height - parent.topPadding * 2
            spacing: 5

            TextField {
                id: add_page_text_field
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40
                placeholderText: qsTr("Page")
            }

            Button {
                id: add_page_button
                text: qsTr("Add Page")
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40

                onClicked: notes_tool.addPage(add_page_text_field.text)
            }

            DelayButton {
                id: delete_page_button
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40
                text: qsTr("Delete Page")

                delay: 1200
                onActivated: notes_tool.deletePage(notes_tool.currentPage)
            }

            Button {
                id: encrypt_button
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40
                text: qsTr("Encrypt Page")

                onClicked: text_edit.text = notes_tool.encrypt(text_edit.text)
            }

            Frame {
                id: pages_text_frame
                width: parent.width
                height: platform.isAndroid ? width / 6 : 40
                padding: 5

                Text {
                    text: qsTr("Pages")
                    color: color_scheme.textColor
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            ScrollView {
                width: parent.width
                height: parent.height - pages_text_frame.height - add_page_button.height
                        - add_page_text_field.height - parent.spacing * 5
                        - delete_page_button.height - encrypt_button.height

                clip: true

                Column {
                    id: page_column
                    width: right_column.width
                    spacing: 5
                }
            }
        }
    }
}
