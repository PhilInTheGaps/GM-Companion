import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import "./tools"
import gm.companion.platforms 1.0

Window {
    id: window
    visible: true
    width: 1280
    height: 720
    title: qsTr("GM-Companion")

    //! [orientation]
    readonly property bool inPortrait: window.width < window.height

    PlatformDetails {
        id: platform
    }

    //! [orientation]
    ToolBar {
        id: toolbar
        width: {
            (inPortrait) ? parent.width : parent.width - drawer.width
        }
        height: 40
        anchors.right: parent.right

        Row {
            id: toolbar_row_left
            height: parent.height

            ToolButton {
                id: tools_button
                text: "Menu"
                height: parent.height
                visible: (inPortrait) ? true : false

                onClicked: drawer.open()
            }
        }

        Label {
            id: tool_label
            text: "Current Tool: Audio"
            height: parent.height
            verticalAlignment: "AlignVCenter"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: toolbar_row_left.verticalCenter
        }
    }

    Rectangle {
        id: drawer_bg
        color: "#1b2224"
    }

    Drawer {
        id: drawer
        width: {
            if (platform.isAndroid) {
                parent.width / 4
            } else {
                180
            }
        }

        height: parent.height
        background: drawer_bg

        modal: inPortrait
        interactive: inPortrait || platform.isAndroid
        position: inPortrait ? 0 : 1
        visible: !inPortrait

        Column {
            anchors.fill: parent
            spacing: 5

            Text {
                text: "Tools"
                color: "white"

                width: parent.width
                height: 40

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Rectangle {
                width: parent.width
                height: 2
                color: "#262f31"
            }

            Button {
                text: "Audio"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/audio.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Audio"
                    swipe.setCurrentIndex(0)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Button {
                text: "Maps"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/maps.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Maps"
                    swipe.setCurrentIndex(1)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Button {
                text: "Dice"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/dice.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Dice"
                    swipe.setCurrentIndex(2)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Button {
                text: "Combat Tracker"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/combat.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Combat"
                    swipe.setCurrentIndex(3)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Button {
                text: "Item Shop"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/item-shop.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Item Shop"
                    swipe.setCurrentIndex(4)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Button {
                text: "Characters"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/characters.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Characters"
                    swipe.setCurrentIndex(5)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Button {
                text: "Generators"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/generators.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Generators"
                    swipe.setCurrentIndex(6)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Button {
                text: "Notes"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/notes.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Notes"
                    swipe.setCurrentIndex(7)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Button {
                text: "Converter"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/converter.png"
                }

                onClicked: {
                    tool_label.text = "Current Tool: Unit Converter"
                    swipe.setCurrentIndex(8)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 2
                color: "#262f31"
            }

            Button {
                text: "Settings"
                width: parent.width
                height: platform.isAndroid ? parent.height / 15 : 45

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    opacity: enabled ? 1.0 : 0.3
                    color: parent.down ? "black" : "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                Image {
                    fillMode: Image.PreserveAspectFit
                    x: 5
                    anchors.verticalCenter: parent.verticalCenter
                    sourceSize.height: parent.height - 10
                    height: sourceSize.height
                    source: "icons/menu/settings.png"
                }

                background: Rectangle {
                    implicitHeight: 40
                    color: parent.down ? "white" : "transparent"
                }

                onClicked: {
                    tool_label.text = "Settings"
                    swipe.setCurrentIndex(9)

                    if (inPortrait) {
                        drawer.close()
                    }
                }
            }
        }
    }

    SwipeView {
        id: swipe
        width: {

            if (inPortrait) {
                parent.width
            } else {
                if (drawer.visible) {
                    parent.width - drawer.width
                } else {
                    parent.width
                }
            }
        }

        height: parent.height - toolbar.height
        anchors.right: parent.right
        y: toolbar.height

        interactive: false
        currentIndex: 0

        Audio {
            id: audio
        }

        Maps {
            id: maps
        }

        Dice {
            id: dice
        }

        CombatTracker {
            id: combat_tracker
        }

        ItemShop {
            id: item_shop
        }

        Characters {
            id: characters
        }

        Generators {
            id: generators
        }

        Notes {
            id: notes
        }

        Converter {
            id: converter
        }

        Settings {
            id: settings
        }
    }
}
