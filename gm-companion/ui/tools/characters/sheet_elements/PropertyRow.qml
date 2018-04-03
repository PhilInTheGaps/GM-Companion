import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

Row {
    id: row
    spacing: 10

    property string property_name: ""
    property string property_value: ""
    property string property_placeholder: ""

    Text {
        id: text

        text: property_name
        anchors.verticalCenter: parent.verticalCenter
    }

    TextField {
        id: text_field
        width: parent.width - text.width - parent.spacing

        text: property_value
        placeholderText: property_placeholder
    }
}
