import QtQuick 2.6
import QtQuick.Controls 2.2
import CustomComponents 1.0
import "../../defines.js" as Defines

Dialog {
    id: root

    property var page: undefined

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
            height: Defines.TOOLBAR_HEIGHT
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
