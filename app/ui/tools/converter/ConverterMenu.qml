import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "../../defines.js" as Defines

Rectangle {
    id: side_bar
    color: palette.alternateBase
    width: Defines.TOOLBAR_WIDTH

    Column {
        anchors.fill: parent
        spacing: 10

        Repeater {
            model: [FontAwesome.ruler, FontAwesome.rulerCombined, FontAwesome.prescriptionBottle, FontAwesome.balanceScale, FontAwesome.dollarSign]

            CustomToolBarButton {
                iconText: modelData
                onClicked: converter_tool.setCurrentType(index)
                verticalMode: true
                outline: true
                height: width
            }
        }
    }
}
