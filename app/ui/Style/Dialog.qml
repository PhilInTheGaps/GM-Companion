import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls
import QtQuick.Controls.impl
import "./colors.js" as Colors

T.Dialog {
    id: control

    implicitWidth: Math.max(
                       background ? background.implicitWidth : 0,
                       header && header.visible ? header.implicitWidth : 0,
                       footer && footer.visible ? footer.implicitWidth : 0,
                       contentWidth > 0 ? contentWidth + leftPadding + rightPadding : 0)
    implicitHeight: Math.max(
                        background ? background.implicitHeight : 0,
                        (header
                         && header.visible ? header.implicitHeight + spacing : 0)
                        + (footer
                           && footer.visible ? footer.implicitHeight + spacing : 0)
                        + (contentHeight > 0 ? contentHeight + topPadding + bottomPadding : 0))

    contentWidth: contentItem.implicitWidth
                  || (contentChildren.length === 1 ? contentChildren[0].implicitWidth : 0)
    contentHeight: contentItem.implicitHeight
                   || (contentChildren.length === 1 ? contentChildren[0].implicitHeight : 0)

    padding: 12

    enter: Transition {
        // grow_fade_in
        NumberAnimation {
            property: "scale"
            from: 0.9
            to: 1.0
            easing.type: Easing.OutQuint
            duration: 200
        }
        NumberAnimation {
            property: "opacity"
            from: 0.0
            to: 1.0
            easing.type: Easing.OutCubic
            duration: 150
        }
    }

    exit: Transition {
        // shrink_fade_out
        NumberAnimation {
            property: "scale"
            from: 1.0
            to: 0.9
            easing.type: Easing.OutQuint
            duration: 200
        }
        NumberAnimation {
            property: "opacity"
            from: 1.0
            to: 0.0
            easing.type: Easing.OutCubic
            duration: 150
        }
    }

    background: Rectangle {
        border.color: Colors.dark
        color: Colors.window
    }

    header: Label {
        text: control.title
        visible: control.title
        elide: Label.ElideRight
        font.bold: true
        padding: 12
        background: Rectangle {
            x: 1
            y: 1
            width: parent.width - 2
            height: parent.height - 1
            color: Colors.alternateBase

            Rectangle {
                color: Colors.dark
                height: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
        }
    }

    footer: DialogButtonBox {
        visible: count > 0
    }
}
