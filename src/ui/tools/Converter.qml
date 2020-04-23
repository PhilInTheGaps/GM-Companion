import QtQuick 2.9
import QtQuick.Controls 2.2
import FontAwesome 2.0
import "./converter"
import "../defines.js" as Defines

Page {
    id: converter

    AddUnitDialog {
        id: add_unit_dialog
    }

    header: ConverterHeader {
        id: header_rect
    }

    ConverterBar {
        id: converter_bar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: side_bar.left
    }

    ConverterUnitPage {
        id: unit_page
        anchors.top: converter_bar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: side_bar.left

        categories: converter_tool.categories
        units: converter_tool.units

        onSetCategory: converter_tool.setCurrentCategory(category)
        onSetUnit: converter_tool.setUnit(unit)
        onDeleteUnit: converter_tool.deleteUnit(unit)
    }

    ConverterMenu {
        id: side_bar
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }
}
