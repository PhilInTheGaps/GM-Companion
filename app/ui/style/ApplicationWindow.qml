import QtQuick 2.0
import QtQuick.Templates 2.2
import "colors.js" as Colors

ApplicationWindow {
    id: window

    color: Colors.window

    overlay.modal: Rectangle {
        color: Colors.dim
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    overlay.modeless: Rectangle {
        color: Colors.dim
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }
}
