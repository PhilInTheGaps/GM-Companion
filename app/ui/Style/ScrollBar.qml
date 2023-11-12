import QtQuick
import QtQuick.Templates as T

T.ScrollBar {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0, contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0, contentItem.implicitHeight + topPadding + bottomPadding)

    padding: 2
    visible: policy !== T.ScrollBar.AlwaysOff

    contentItem: Rectangle {
        implicitWidth: control.interactive ? 6 : 2
        implicitHeight: control.interactive ? 6 : 2

        radius: width / 2
        color: control.pressed ? StyleColors.buttonPressed : palette.button
        opacity: control.size < 1.0 ? 0.5 : 0.0

        states: State {
            name: "active"
            when: control.policy === T.ScrollBar.AlwaysOn || (control.active && control.size < 1.0)

            PropertyChanges {
                control.contentItem.opacity: 0.75
            }
        }

        transitions: Transition {
            from: "active"

            SequentialAnimation {
                PauseAnimation {
                    duration: 450
                }

                NumberAnimation {
                    target: control.contentItem
                    duration: 200
                    property: "opacity"
                    to: 0.5
                }
            }
        }
    }
}
