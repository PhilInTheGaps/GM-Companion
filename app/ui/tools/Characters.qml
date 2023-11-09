import QtQuick
import QtQuick.Controls
import CustomComponents
import src
import "./characters"

Page {
    id: root

    Component.onCompleted: CharacterTool.loadData()

    header: ToolBar {
        id: top_rect

        Row {
            id: top_left_row
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            padding: 10
            spacing: 10

            CheckBox {
                text: qsTr("Active Characters")
                checked: true
                onClicked: CharacterTool.displayActiveCharacters(checked)
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        // Image buttons
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Repeater {
                model: CharacterTool.categories

                CustomToolBarButton {
                    id: category_delegate
                    required property string modelData
                    required property int index

                    buttonText: modelData
                    onClicked: CharacterTool.setCurrentCategory(index)
                    usesFixedWidth: false
                    pointSize: 12

                    Rectangle {
                        visible: category_delegate.index === CharacterTool.categoryIndex

                        color: palette.text
                        height: 1
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 4
                    }
                }
            }
        }
    }

    contentItem: SplitView {
        id: split_view
        orientation: Qt.Horizontal

        // Left bar
        CharacterList {
            id: character_list
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            SplitView.minimumWidth: 160
            SplitView.preferredWidth: 175
        }

        SwipeView {
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            SplitView.minimumWidth: 300
            SplitView.fillWidth: true

            clip: true

            currentIndex: CharacterTool.pageIndex
            interactive: false

            Loader {
                id: classic_page
                source: "characters/classic/ClassicPage.qml"
                asynchronous: true
                active: true
            }
        }
    }
}
