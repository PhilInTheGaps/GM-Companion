import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0

SpinBox {
    id: box
    width: parent.width / 6
    height: parent.height - parent.padding * 2
    editable: true
    value: field_value

    property string text_color: white
    property int field_value: 0
    property bool current_item: false

    background: Rectangle {
        color: "transparent"
    }

    contentItem: TextInput {
        text: Number.fromLocaleString(parent.locale, field_value)

        onTextChanged: {
            try {
                var new_health = Number.fromLocaleString(parent.locale, text)

                if (new_health < -100) {
                    new_health = 0
                    text = Number(new_health).toString()
                }
            } catch (error) {
                new_health = 0
                text = Number(new_health).toString()
            }

            field_value = new_health
        }

        color: text_color
        font.pointSize: 12
        font.bold: true

        inputMethodHints: Qt.ImhFormattedNumbersOnly
        validator: parent.validator
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
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

        Text {
            text: FontAwesome.minus
            font.family: FontAwesome.familySolid
            font.pointSize: 20
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

        Text {
            text: FontAwesome.plus
            font.family: FontAwesome.familySolid
            font.pointSize: 20
            color: box.to <= field_value ? "lightgrey" : parent.pressed ? "grey" : parent.hovered ? "lightgrey" : current_item ? "white" : color_scheme.primaryButtonColor
            anchors.centerIn: parent
        }

        enabled: box.to > field_value

        onClicked: field_value++
    }
}
