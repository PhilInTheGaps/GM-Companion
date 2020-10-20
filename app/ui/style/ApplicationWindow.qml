import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Controls.impl 2.2
import QtQuick.Templates 2.2 as T
import "../colors.js" as Colors

T.ApplicationWindow {
    id: window

    color: Colors.window

    overlay.modal: Rectangle {
        color: Colors.dim
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }

    overlay.modeless: Rectangle {
        color: Colors.dim
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }
}
