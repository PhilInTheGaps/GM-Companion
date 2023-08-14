import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import CustomComponents 1.0
import IconFonts
import "../../defines.js" as Defines

Page {
    id: root

    Component.onCompleted: name_generator.loadData()

    Rectangle {
        id: left_column

        color: palette.dark
        width: Defines.SIDEBAR_WIDTH

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        CustomComboBox {
            id: left_combo_box
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5

            model: name_generator ? name_generator.categories : []
            emptyString: name_generator
                         && name_generator.isLoading ? qsTr("Loading ...") : qsTr(
                                                           "No Names")

            onCurrentIndexChanged: {
                if (name_generator) {
                    name_generator.loadCategory(currentIndex)
                }
            }
        }

        ScrollView {
            id: category_view

            anchors.top: left_combo_box.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 5

            clip: true
            contentWidth: -1
            contentHeight: generators_structure.implicitHeight

            Column {
                id: generators_structure
                anchors.left: parent.left
                anchors.right: parent.right

                Repeater {
                    id: generators_repeater

                    model: name_generator ? name_generator.generators : []

                    CustomButton {
                        buttonText: modelData.name

                        anchors.left: parent.left
                        anchors.right: parent.right

                        onClicked: {
                            if (name_generator) {
                                if (name_generator.loadGenerator(index)) {
                                    name_generator.currentGenerator.generate(
                                                count_spinbox.value)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Item {
        id: mid

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: left_column.right
        anchors.right: right_column.left
        anchors.margins: 10

        RowLayout {
            anchors.fill: parent
            spacing: 10

            Repeater {
                id: mid_repeater
                model: name_generator
                       && name_generator.currentGenerator ? name_generator.currentGenerator.categories : []

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: text_area.text.length > 0

                    Label {
                        id: header
                        text: modelData
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        font.pointSize: 12
                    }

                    TextArea {
                        id: text_area
                        anchors.top: header.bottom
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.topMargin: 10

                        selectByMouse: true
                        padding: 0
                        font.pointSize: 12
                        wrapMode: TextArea.WrapAtWordBoundaryOrAnywhere

                        text: name_generator
                              && name_generator.currentGenerator ? name_generator.currentGenerator.generatedNames[index].join(
                                                                       "\n") : ""
                    }
                }
            }
        }
    }

    Rectangle {
        id: right_column

        color: palette.dark
        width: Defines.SIDEBAR_WIDTH

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Column {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 20

            CustomButton {
                iconText: FontAwesome.rotate
                buttonText: qsTr("Refresh")
                anchors.left: parent.left
                anchors.right: parent.right

                onClicked: {
                    if (name_generator && name_generator.currentGenerator) {
                        name_generator.currentGenerator.generate(
                                    count_spinbox.value)
                    }
                }
            }

            Column {
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Count")
                    font.bold: true
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                SpinBox {
                    id: count_spinbox
                    from: 1
                    value: 15
                    editable: true
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }

            Column {
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Categories")
                    font.bold: true
                    anchors.left: parent.left
                    anchors.right: parent.right
                    visible: generator_categories_repeater.count > 0
                }

                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 10

                    Repeater {
                        id: generator_categories_repeater
                        model: name_generator
                               && name_generator.currentGenerator ? name_generator.currentGenerator.categories : []

                        CheckBox {
                            text: modelData
                            anchors.left: parent.left
                            anchors.right: parent.right
                            checked: name_generator
                                     && name_generator.currentGenerator ? name_generator.currentGenerator.enabledCategories[index] : true

                            onClicked: {
                                if (name_generator
                                        && name_generator.currentGenerator) {
                                    name_generator.currentGenerator.setCategoryEnabled(
                                                index, checked)
                                }
                            }
                        }
                    }
                }
            }

            Column {
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Prefixes")
                    font.bold: true
                    anchors.left: parent.left
                    anchors.right: parent.right
                    visible: generator_prefixes_repeater.count > 0
                }

                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Repeater {
                        id: generator_prefixes_repeater
                        model: name_generator
                               && name_generator.currentGenerator ? name_generator.currentGenerator.prefixes : []

                        RadioButton {
                            text: modelData
                            anchors.left: parent.left
                            anchors.right: parent.right
                            checked: name_generator
                                     && name_generator.currentGenerator ? name_generator.currentGenerator.activePrefix === index : false

                            onClicked: {
                                if (name_generator
                                        && name_generator.currentGenerator) {
                                    name_generator.currentGenerator.activePrefix = index
                                }
                            }
                        }
                    }
                }
            }

            Column {
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right

                Label {
                    text: qsTr("Suffixes")
                    font.bold: true
                    anchors.left: parent.left
                    anchors.right: parent.right
                    visible: generator_suffixes_repeater.count > 0
                }

                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Repeater {
                        id: generator_suffixes_repeater
                        model: name_generator
                               && name_generator.currentGenerator ? name_generator.currentGenerator.suffixes : []

                        RadioButton {
                            text: modelData
                            anchors.left: parent.left
                            anchors.right: parent.right
                            checked: name_generator
                                     && name_generator.currentGenerator ? name_generator.currentGenerator.activeSuffix === index : false

                            onClicked: {
                                if (name_generator
                                        && name_generator.currentGenerator) {
                                    name_generator.currentGenerator.activeSuffix = index
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
