pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import src
import "./converter"

Page {
    id: converter

    Component.onCompleted: ConverterTool.loadData()

    StackView {
        id: stack
        anchors.fill: parent
        clip: true
        initialItem: converter_page
    }

    ConverterPage {
        id: converter_page

        onOpenEditor: {
            stack.push(converter_editor_component)
        }
    }

    Component {
        id: converter_editor_component

        ConverterEditorPage {
            id: converter_editor_page

            onBackToTool: {
                stack.pop()
            }
        }
    }
}
