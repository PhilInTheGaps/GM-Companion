import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

import gm.companion.notestool 1.0
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
            text_edit.text = ""

            if (pages.length > 0) {
                setCurrentPage(pages[0])
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

    Row {
        anchors.fill: parent
        spacing: 5
        padding: 5

        Column {
            id: left_column
            height: parent.height - parent.topPadding * 2
            width: 150
            spacing: 5

            Frame {
                id: font_size_frame
                width: parent.width
                padding: 5

                Row {
                    width: parent.width
                    spacing: 5

                    Text {
                        id: font_size_text
                        text: qsTr("Font Size:")
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    SpinBox {
                        id: font_size_spin_box
                        value: 12
                        width: parent.width - parent.spacing - font_size_text.width

                        onValueChanged: text_edit.font.pointSize = value
                    }
                }
            }

            Row {
                id: add_chapter_row
                width: parent.width
                spacing: 5

                Button {
                    id: add_chapter_button
                    text: qsTr("Add Chapter")

                    onClicked: notes_tool.addChapter(
                                   add_chapter_text_field.text)
                }

                TextField {
                    id: add_chapter_text_field
                    width: parent.width - add_chapter_button.width - parent.spacing
                    placeholderText: qsTr("Chapter")
                }
            }

            Frame {
                id: chapters_text_frame
                width: parent.width
                padding: 5

                Text {
                    text: qsTr("Chapters")
                }
            }

            Column {
                id: chapter_column
                width: parent.width
                height: parent.height - font_size_frame.height - chapters_text_frame.height
                        - delete_chapter_button.height - add_chapter_row.height - parent.spacing * 4

                spacing: 5
            }

            Button {
                id: delete_chapter_button
                width: parent.width

                text: qsTr("Delete Chapter")

                onClicked: notes_tool.deleteChapter(notes_tool.currentChapter)
            }
        }

        Column {
            id: text_edit_column
            height: parent.height - parent.topPadding * 2
            width: parent.width - left_column.width - parent.spacing * 2
                   - parent.leftPadding * 2 - right_column.width

            TextArea {
                id: text_edit
                width: parent.width
                height: parent.height

                selectByMouse: true
                wrapMode: TextEdit.WordWrap

                onTextChanged: notes_tool.saveCurrentPageContent(text)
            }
        }

        Column {
            id: right_column
            width: 150
            height: parent.height - parent.topPadding * 2
            spacing: 5

            Row {
                id: add_page_row
                width: parent.width
                spacing: 5

                Button {
                    id: add_page_button
                    text: qsTr("Add Page")

                    onClicked: notes_tool.addPage(add_page_text_field.text)
                }

                TextField {
                    id: add_page_text_field
                    width: parent.width - add_page_button.width - parent.spacing
                    placeholderText: qsTr("Page")
                }
            }

            Button {
                id: encrypt_button
                width: parent.width
                text: qsTr("Encrypt Page")

                onClicked: text_edit.text = notes_tool.encrypt(text_edit.text)
            }

            Frame {
                id: pages_text_frame
                width: parent.width
                padding: 5

                Text {
                    text: qsTr("Pages")
                }
            }

            ScrollView {
                width: parent.width
                height: parent.height - pages_text_frame.height - add_page_row.height
                        - parent.spacing * 4 - delete_page_button.height - encrypt_button.height

                flickableItem.interactive: true
                flickableItem.flickableDirection: Flickable.VerticalFlick
                clip: true

                Column {
                    id: page_column
                    width: right_column.width - right_column.padding * 2
                    spacing: 5
                }
            }

            Button {
                id: delete_page_button
                width: parent.width

                text: qsTr("Delete Page")

                onClicked: notes_tool.deletePage(notes_tool.currentPage)
            }
        }
    }
}
