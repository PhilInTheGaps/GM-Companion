import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../../components"

Column {
    padding: 20
    spacing: 10

    Repeater {
        model: character_dsa5_viewer.special

        Label {
            text: modelData
            color: color_scheme.textColor
            font.pointSize: 12
        }
    }
}
