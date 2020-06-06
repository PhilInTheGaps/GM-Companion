import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../../components"

Item {
    DSA5ItemList {
        anchors.fill: parent
        model: dsa5LiturgiesModel
        value_fields: 4
    }

    Label {
        text: "// TODO"
        anchors.centerIn: parent
        color: color_scheme.textColor
    }
}
