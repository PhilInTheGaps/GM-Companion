import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Templates as T

T.ApplicationWindow {
    id: window

    color: palette.window

    Overlay.overlay.modal: Rectangle {
        color: StyleColors.dim
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }

    Overlay.overlay.modeless: Rectangle {
        color: StyleColors.dim
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }
}
