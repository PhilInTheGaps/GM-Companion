import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root
    property string placeholderText: "New Thing"
    signal done(string thing)

    contentItem: Column {
        TextField {
            id: textfield
            placeholderText: root.placeholderText
            selectByMouse: true
        }
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        done(textfield.text)
        textfield.clear()
    }
}
