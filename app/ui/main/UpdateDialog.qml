import QtQuick 2.9
import QtQuick.Controls 2.2

Dialog {
    id: root

    title: qsTr("Update Available!")

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true

    Column {
        spacing: 10

        Label {
            text: qsTr("New Version:")
        }

        Label {
            text: update_manager.newestVersion
        }

        Row {
            spacing: 10

            Button {
                text: qsTr("Download")
                onClicked: Qt.openUrlExternally(
                               "https://github.com/PhilInTheGaps/GM-Companion/releases")
            }

            Button {
                text: qsTr("Later")
                onClicked: root.close()
            }
        }
    }
}
