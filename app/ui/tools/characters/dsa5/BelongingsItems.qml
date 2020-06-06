import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../../components"

Item {
    DSA5ItemList {
        anchors.fill: parent
        model: dsa5ItemsModel
        value_fields: 1
        spacer_indices: []
    }
}
