import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1

import gm.companion.namegeneratortool 1.0

Item {
    NameGeneratorTool {
        id: name_generator_tool

        Component.onCompleted: loadCategories()

        onCategoriesChanged: {
            loadCategories()
        }

        function loadCategories() {
            names_categories_column.children = []

            for (var i = 0; i < categories.length; i++) {
                var component = Qt.createComponent(
                            "NameGeneratorCategoryButton.qml")

                var button = component.createObject(names_categories_column, {
                                                        category: categories[i],
                                                        max_width: left_column.width
                                                    })

                button.clicked.connect(loadNames)
            }
        }

        function loadNames(category) {
            names_flow.children = []

            var names = categoryNames(category)

            for (var i = 0; i < names.length; i++) {
                var component = Qt.createComponent("NameGeneratorButton.qml")

                var button = component.createObject(names_flow, {
                                                        name: names[i],
                                                        category: category,
                                                        path: categoryPath(
                                                                  category) + "/" + names[i]
                                                    })

                button.clicked.connect(displayGeneratedNames)
            }
        }

        function displayGeneratedNames(category, name) {
            male_names_text_area.cursorPosition = 0
            female_names_text_area.cursorPosition = 0

            male_names_text_area.text = ""
            female_names_text_area.text = ""

            var count = names_count_spinbox.value
            var male_names = maleNames(category, name, count)
            var female_names = femaleNames(category, name, count)

            for (var i = 0; i < count; i++) {
                male_names_text_area.append(male_names[i])
                female_names_text_area.append(female_names[i])
            }
        }
    }

    Row {
        anchors.fill: parent
        spacing: 5
        padding: 5

        Column {
            id: left_column
            width: parent.width / 8
            height: parent.height - parent.topPadding * 2
            spacing: 5

            Text {
                id: names_count_text
                text: qsTr("Generated Names")
                color: color_scheme.textColor
            }

            SpinBox {
                id: names_count_spinbox
                width: parent.width
                value: 15
                from: 1
                editable: true
            }

            Text {
                id: names_font_size_text
                text: qsTr("Font Size")
                color: color_scheme.textColor
            }

            SpinBox {
                id: names_font_size_spinbox
                width: parent.width
                value: 20
                from: 1
                editable: true

                onValueChanged: setPointSize()
                Component.onCompleted: setPointSize()

                function setPointSize() {
                    male_names_text_area.font.pointSize = value
                    female_names_text_area.font.pointSize = value
                }
            }

            Text {
                id: names_categories_text
                text: qsTr("Name Categories")
                color: color_scheme.textColor
            }

            ScrollView {
                id: names_scroll_view
                width: parent.width
                height: parent.height - names_count_text.height - names_count_spinbox.height
                        - names_font_size_text.height - names_font_size_spinbox.height
                        - names_categories_text.height - parent.spacing * 5

                clip: true

                Column {
                    id: names_categories_column
                    width: parent.parent.width
                    spacing: 5
                }
            }
        }

        Column {
            id: mid_column
            width: (parent.width - left_column.width - parent.leftPadding * 2 - parent.spacing)
            height: parent.height - parent.topPadding * 2
            spacing: 5

            Flow {
                id: names_flow
                width: parent.width
                spacing: 5
            }

            Row {
                width: parent.width
                height: parent.height - names_flow.height - parent.spacing
                spacing: 5

                Column {
                    id: male_names_column
                    width: (parent.width - parent.spacing) / 2
                    height: parent.height
                    spacing: 5

                    Text {
                        id: male_names_text
                        text: qsTr("Male Names")
                        color: color_scheme.textColor
                    }

                    TextArea {
                        id: male_names_text_area
                        width: parent.width
                        height: parent.height - male_names_text.height - parent.spacing

                        color: color_scheme.textColor
                        readOnly: true
                        selectByMouse: true
                    }
                }

                Column {
                    id: female_names_column
                    width: (parent.width - parent.spacing) / 2
                    height: parent.height
                    spacing: 5

                    Text {
                        id: female_names_text
                        text: qsTr("Female Names")
                        color: color_scheme.textColor
                    }

                    TextArea {
                        id: female_names_text_area
                        width: parent.width
                        height: parent.height - female_names_text.height - parent.spacing
                        color: color_scheme.textColor

                        readOnly: true
                        selectByMouse: true
                    }
                }
            }
        }
    }
}
