import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4

Button {
    property var tool_name
    property var icon_source

    id: button
    width: parent.width
    height: platform.isAndroid ? parent.height / 15 : 45

    background: Rectangle {
        implicitHeight: 40
        color: parent.down ? "white" : "transparent"
    }

    Row {
        anchors.fill: parent
        padding: 5
        spacing: 5

        Image {
            id: button_icon
            fillMode: Image.PreserveAspectFit
            anchors.verticalCenter: parent.verticalCenter
            sourceSize.height: parent.height - 10
            height: sourceSize.height
            source: icon_source
        }

        Rectangle {
            height: parent.height - parent.topPadding * 2
            width: parent.width - button_icon.width - parent.spacing - parent.leftPadding * 2
            color: "transparent"

            Text {
                text: tool_name
                font: button.font
                opacity: enabled ? 1.0 : 0.3
                color: button.down ? "black" : "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                elide: Text.ElideRight
            }
        }
    }
}
