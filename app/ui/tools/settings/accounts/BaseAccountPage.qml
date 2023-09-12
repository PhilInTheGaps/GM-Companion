import QtQuick
import QtQuick.Controls
import IconFonts
import services
import "./../../.."

Item {
    id: root

    property string name: "Unknown Account"
    property string icon: FontAwesome.users
    property font iconFont: FontAwesome.fontSolid
    property bool isExperimental: false

    property alias leftPanel: left_loader.sourceComponent
    property alias rightPanel: right_loader.sourceComponent

    property var statuses

    Row {
        id: header

        topPadding: 5
        spacing: 5

        anchors.left: parent.left
        anchors.right: parent.right
        height: Sizes.toolbarHeight

        Label {
            id: header_icon_label
            text: root.icon
            font.family: root.iconFont.family
            font.styleName: root.iconFont.styleName
            font.pointSize: 12
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
        }

        Label {
            id: header_name_label
            text: root.name
            font.bold: true
            font.pointSize: 12
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
        }

        Label {
            id: header_additional_infolabel
            text: qsTr("[Experimental Feature]")
            font.bold: false
            font.pointSize: 10
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            visible: root.isExperimental
            color: "orange"
        }
    }

    Item {
        id: body

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: header.bottom
        anchors.bottom: footer.top
        anchors.bottomMargin: 10

        Loader {
            id: left_loader

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: root.rightPanel
                   || parent.width > 1200 ? parent.width / 2 - 10 : parent.width
        }

        Rectangle {
            width: 1
            color: Colors.border
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: left_loader.right
            anchors.margins: 10
            visible: right_loader.status === Loader.Ready
        }

        Loader {
            id: right_loader

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: left_loader.right
            anchors.right: parent.right
            anchors.leftMargin: 21
        }
    }

    Column {
        id: footer
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 5

        Repeater {
            model: root.statuses

            StatusBar {
                id: status_label

                required property ServiceStatus modelData

                anchors.left: parent.left
                anchors.right: parent.right
                status: modelData
            }
        }
    }
}
