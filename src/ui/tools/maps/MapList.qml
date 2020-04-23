import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0

Rectangle {
    id: root

    color: palette.dark

    ScrollView {
        id: scroll_view
        anchors.fill: parent
        anchors.margins: 5
        clip: true
        contentWidth: -1

        Column {
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: map_tool.categories

                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    spacing: 10

                    CustomButton {
                        id: button
                        anchors.left: parent.left
                        anchors.right: parent.right

                        buttonText: modelData

                        onClicked: {
                            map_tool.setCurrentCategory(index)
                            //                            column.visible = !column.visible
                        }
                    }

                    Column {
                        id: column
                        anchors.left: parent.left
                        anchors.right: parent.right
                        spacing: 10

                        visible: map_tool.currentCategory === index

                        Repeater {
                            model: column.visible ? mapListModel : []

                            MapListItem {
                                name: modelData.name
                                path: modelData.path
                                imageData: modelData.imageData
                                markers: modelData.markers
                                hasMarkers: modelData.hasMarkers
                                mapIndex: index
                            }
                        }
                    }
                }
            }
        }
    }
}
