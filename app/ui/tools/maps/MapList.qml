import QtQuick
import QtQuick.Controls
import CustomComponents
import src

Rectangle {
    id: root

    color: palette.dark

    Flickable {
        id: flickable
        anchors.fill: parent
        anchors.margins: 5
        clip: true
        contentHeight: flickable_column.height
        contentWidth: -1

        ScrollBar.vertical: ScrollBar {
            anchors.right: parent.right
            visible: flickable.contentHeight > flickable.height
        }

        Column {
            id: flickable_column
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: MapTool.categories

                Column {
                    id: category_delegate
                    required property string modelData
                    required property int index

                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 10

                    CustomButton {
                        id: button
                        anchors.left: parent.left
                        anchors.right: parent.right

                        buttonText: category_delegate.modelData

                        onClicked: {
                            MapTool.setCurrentCategory(category_delegate.index)
                        }
                    }

                    Column {
                        id: column
                        anchors.left: parent.left
                        anchors.right: parent.right
                        spacing: 10

                        visible: MapTool.currentCategory === category_delegate.index

                        Repeater {
                            model: column.visible ? MapTool.listModel : []

                            MapListItem {
                                required property int index
                                required property Map modelData

                                map: modelData
                                mapIndex: index
                            }
                        }
                    }
                }
            }
        }
    }
}
