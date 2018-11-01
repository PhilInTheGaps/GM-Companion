import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../fontawesome"

SpinBox {
    id: box
    width: parent.width / 6
    height: parent.height - parent.padding * 2
    editable: false
    value: field_value

    property string text_color: white
    property int field_value: 0
    property bool current_item: false

    background: Rectangle {
        color: "transparent"
    }

    contentItem: TextInput {
        text: (textFromValue(field_value, parent.locale))
        color: text_color
        inputMethodHints: Qt.ImhFormattedNumbersOnly
        font.pointSize: 12
        font.bold: true
        validator: parent.validator
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        onTextChanged: field_value = (valueFromText(text, locale))
        anchors.centerIn: parent
        width: parent.width - up_button.width - down_button.width - 10
        readOnly: !parent.editable
    }

    down.indicator: Button {
        id: down_button
        height: parent.height
        width: height
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        hoverEnabled: true

        background: Rectangle {
            color: "transparent"
        }

        Icon {
            icon: icons.fas_minus
            pointSize: 20
            color: box.from >= field_value ? "lightgrey" : parent.pressed ? "grey" : parent.hovered ? "lightgrey" : current_item ? "white" : color_scheme.primaryButtonColor
            anchors.centerIn: parent
        }

        enabled: box.from < field_value

        onClicked: field_value--
    }

    up.indicator: Button {
        id: up_button
        height: parent.height
        width: height
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        hoverEnabled: true

        background: Rectangle {
            color: "transparent"
        }

        Icon {
            icon: icons.fas_plus
            pointSize: 20
            color: box.to <= field_value ? "lightgrey" : parent.pressed ? "grey" : parent.hovered ? "lightgrey" : current_item ? "white" : color_scheme.primaryButtonColor
            anchors.centerIn: parent
        }

        enabled: box.to > field_value

        onClicked: field_value++
    }
}
