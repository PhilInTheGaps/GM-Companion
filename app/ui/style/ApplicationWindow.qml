import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T
import "../colors.js" as Colors

T.ApplicationWindow {
    id: window

    color: Colors.window

    Overlay.overlay.modal: Rectangle {
        color: Colors.dim
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }

    Overlay.overlay.modeless: Rectangle {
        color: Colors.dim
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }
}
