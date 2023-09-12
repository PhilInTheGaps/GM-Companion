import QtQuick
import QtQuick.Templates as T

T.ScrollIndicator {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        contentItem.implicitHeight + topPadding + bottomPadding)

    padding: 2

    contentItem: Rectangle {
        implicitWidth: 2
        implicitHeight: 2

        color: palette.button
        visible: control.size < 1.0
        opacity: 0.0

        states: State {
            name: "active"
            when: control.active
            PropertyChanges {
                control.contentItem.opacity: 0.75
            }
        }

        transitions: [
            Transition {
                from: "active"
                SequentialAnimation {
                    PauseAnimation {
                        duration: 450
                    }
                    NumberAnimation {
                        target: control.contentItem
                        duration: 200
                        property: "opacity"
                        to: 0.0
                    }
                }
            }
        ]
    }
}
