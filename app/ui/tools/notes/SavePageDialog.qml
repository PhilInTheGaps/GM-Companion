import QtQuick
import QtQuick.Controls
import CustomComponents
import src
import "../.."

Dialog {
    id: root

    property NoteBookPage page

    modal: true

    background: Rectangle {
        color: palette.dark
    }

    onAccepted: {
        if (page) {
            page.save()
            page.close()
        }
    }

    onRejected: {
        if (page) {
            page.close()
        }
    }

    contentItem: Column {
        Label {
            // qmllint disable missing-property
            text: qsTr("Do you want to save your changes to ")
                  + (root.page ? root.page.name : "UNDEFINED") + "?"
            font.bold: true
        }

        Label {
            text: qsTr("Otherwise the changes will be lost.")
        }

        Item {
            width: 1
            height: 10
        }

        Row {
            id: buttons
            anchors.right: parent.right
            height: Sizes.toolbarHeight
            spacing: 5

            CustomButton {
                id: yes_button
                buttonText: qsTr("Yes")
                usesFixedWidth: false
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                leftPadding: 10
                rightPadding: 10
                width: Math.max(yes_button.implicitWidth,
                                no_button.implicitWidth)
                onClicked: {
                    root.accept()
                }
            }

            CustomButton {
                id: no_button
                buttonText: qsTr("No")
                usesFixedWidth: false
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                leftPadding: 10
                rightPadding: 10
                width: Math.max(yes_button.implicitWidth,
                                no_button.implicitWidth)
                onClicked: root.reject()
            }
        }
    }
}
