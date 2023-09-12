pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import IconFonts
import src
import "../.."

Page {
    id: root

    Component.onCompleted: NameGeneratorTool.loadData()

    Rectangle {
        id: left_column

        color: palette.dark
        width: Sizes.sidebarWidth

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        CustomComboBox {
            id: left_combo_box
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5

            model: NameGeneratorTool.categories
            emptyString: NameGeneratorTool.isLoading ? qsTr("Loading ...") : qsTr(
                                                           "No Names")

            onCurrentIndexChanged: {
                NameGeneratorTool.loadCategory(currentIndex)
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

                    model: NameGeneratorTool.generators

                    CustomButton {
                        required property AbstractNameGenerator modelData
                        required property int index

                        buttonText: modelData.name

                        anchors.left: parent.left
                        anchors.right: parent.right

                        onClicked: {
                            if (NameGeneratorTool.loadGenerator(index)) {
                                NameGeneratorTool.currentGenerator.generate(count_spinbox.value)
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
                model: NameGeneratorTool.currentGenerator ? NameGeneratorTool.currentGenerator.categories : []

                Item {
                    id: category_delegate

                    required property string modelData
                    required property int index

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: text_area.text.length > 0

                    Label {
                        id: header
                        text: category_delegate.modelData
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

                        // qmllint disable unresolved-type
                        text: NameGeneratorTool.currentGenerator ? NameGeneratorTool.currentGenerator.generatedNames[category_delegate.index].join("\n") : ""
                    }
                }
            }
        }
    }

    Rectangle {
        id: right_column

        color: palette.dark
        width: Sizes.sidebarWidth

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
                    if (NameGeneratorTool.currentGenerator) {
                        NameGeneratorTool.currentGenerator.generate(
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
                        model: NameGeneratorTool.currentGenerator ? NameGeneratorTool.currentGenerator.categories : []

                        CheckBox {
                            required property string modelData
                            required property int index

                            text: modelData
                            anchors.left: parent ? parent.left : undefined
                            anchors.right: parent ? parent.right : undefined
                            checked: NameGeneratorTool.currentGenerator ? NameGeneratorTool.currentGenerator.enabledCategories[index] : true

                            onClicked: {
                                if (NameGeneratorTool.currentGenerator) {
                                    NameGeneratorTool.currentGenerator.setCategoryEnabled(
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
                        model: NameGeneratorTool.currentGenerator ? NameGeneratorTool.currentGenerator.prefixes : []

                        RadioButton {
                            required property string modelData
                            required property int index

                            text: modelData
                            anchors.left: parent ? parent.left : undefined
                            anchors.right: parent ? parent.right : undefined
                            checked: NameGeneratorTool.currentGenerator ? NameGeneratorTool.currentGenerator.activePrefix === index : false

                            onClicked: {
                                if (NameGeneratorTool.currentGenerator) {
                                    NameGeneratorTool.currentGenerator.activePrefix = index
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
                        model: NameGeneratorTool.currentGenerator ? NameGeneratorTool.currentGenerator.suffixes : []

                        RadioButton {
                            required property string modelData
                            required property int index

                            text: modelData
                            anchors.left: parent ? parent.left : undefined
                            anchors.right: parent ? parent.right : undefined
                            checked: NameGeneratorTool.currentGenerator ? NameGeneratorTool.currentGenerator.activeSuffix === index : false

                            onClicked: {
                                if (NameGeneratorTool.currentGenerator) {
                                    NameGeneratorTool.currentGenerator.activeSuffix = index
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
