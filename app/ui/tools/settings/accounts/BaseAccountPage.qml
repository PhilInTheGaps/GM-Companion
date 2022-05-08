import QtQuick 2.9
import QtQuick.Controls 2.2
import CustomComponents 1.0
import FontAwesome 2.0
import "./../../../defines.js" as Defines
import "./../../../colors.js" as Colors

Item {
    id: root

    property string name: "Unknown Account"
    property string icon: FontAwesome.users
    property string iconFamily: FontAwesome.familySolid
    property bool isExperimental: false

    property alias leftPanel: left_loader.sourceComponent
    property alias rightPanel: right_loader.sourceComponent

    property var status: undefined

    Row {
        id: header

        topPadding: 5
        spacing: 5

        anchors.left: parent.left
        anchors.right: parent.right
        height: Defines.TOOLBAR_HEIGHT

        Label {
            id: header_icon_label
            text: root.icon
            font.family: root.iconFamily
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
            model: root.status

            StatusBar {
                id: status_label
                anchors.left: parent.left
                anchors.right: parent.right
                status: modelData
            }
        }
    }
}
