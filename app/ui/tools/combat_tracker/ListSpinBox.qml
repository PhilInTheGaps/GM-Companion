import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

Item {
    id: root

    property string value
    property bool edit_mode: false
    property string font_color: "black"

    signal valueEdited(string new_value)
    signal valueIncreased(int steps)

    Label {
        id: label
        text: root.value
        anchors.centerIn: parent
        color: root.font_color
        font.pixelSize: parent.height - 30

        verticalAlignment: Text.AlignHCenter
        horizontalAlignment: Text.AlignVCenter
    }

    Dialog {
        id: input_item
        visible: edit_mode

        width: parent.width
        height: parent.height
        modal: true
        padding: 0

        onClosed: {
            edit_mode = false
        }

        onAccepted: {
            root.valueEdited(textfield.text)
        }

        TextField {
            id: textfield
            text: root.value
            anchors.fill: parent
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            font.pointSize: 12

            validator: IntValidator {
                top: 99999
                bottom: -99999
            }

            horizontalAlignment: Text.AlignHCenter

            onAccepted: input_item.accept()
        }

        Text {
            id: edit_icon
            text: FontAwesome.checkCircle
            font.family: FontAwesome.familySolid
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 10
            font.pixelSize: parent.height - 30
            color: "green"
        }

        MouseArea {
            id: edit_mouse_area
            onClicked: input_item.accept()
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: edit_icon.width + edit_icon.anchors.margins
        }
    }

    Item {
        id: left_button
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 10
        visible: !edit_mode
        width: left_icon.width + 20

        Text {
            id: left_icon
            text: FontAwesome.minus
            font.family: FontAwesome.familySolid
            font.pointSize: 13
            color: left_area.pressed ? "black" : left_area.containsMouse ? "grey" : font_color
            anchors.centerIn: parent
        }

        MouseArea {
            id: left_area
            anchors.fill: parent
            hoverEnabled: true
            onClicked: valueIncreased(-1)
        }
    }

    Item {
        id: right_button
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 10
        visible: !edit_mode
        width: right_icon.width + 20

        Text {
            id: right_icon
            text: FontAwesome.plus
            font.family: FontAwesome.familySolid
            font.pointSize: 13
            color: right_area.pressed ? "black" : right_area.containsMouse ? "grey" : font_color
            anchors.centerIn: parent
        }

        MouseArea {
            id: right_area
            anchors.fill: parent
            hoverEnabled: true
            onClicked: valueIncreased(1)
        }
    }

    MouseArea {
        id: mouse_area
        visible: !edit_mode
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: left_button.right
        anchors.right: right_button.left
        anchors.margins: 10
        cursorShape: Qt.IBeamCursor

        onClicked: {
            root.edit_mode = true
            textfield.forceActiveFocus()
        }
    }
}
