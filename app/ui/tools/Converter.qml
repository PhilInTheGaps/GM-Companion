import QtQuick 2.9
import QtQuick.Controls 2.2
import "./converter"

Page {
    id: converter

    Component.onCompleted: converter_tool.loadData()

    StackView {
        id: stack
        anchors.fill: parent
        clip: true
        initialItem: converter_page
    }

    ConverterTool {
        id: converter_page

        onOpenEditor: {
            stack.push(converter_editor_component)
        }
    }

    Component {
        id: converter_editor_component

        ConverterEditor {
            id: converter_editor_page

            onBackToTool: {
                stack.pop()
            }
        }
    }
}
