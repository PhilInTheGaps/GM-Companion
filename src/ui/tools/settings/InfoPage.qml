import QtQuick 2.9
import QtQuick.Controls 2.2

Item {
    id: root

    Column {
        id: left_column
        spacing: 5
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        width: 200

        property var button_names: [qsTr("Open Wiki"), qsTr(
                "Report a Bug"), qsTr("Download older Versions"), qsTr(
                "View on GitHub"), qsTr("Changelog"), qsTr("About")]
        property var button_urls: ["https://github.com/PhilInTheGaps/GM-Companion/wiki", "https://github.com/PhilInTheGaps/GM-Companion/issues", "https://github.com/PhilInTheGaps/GM-Companion/releases", "https://github.com/PhilInTheGaps/GM-Companion", "https://github.com/PhilInTheGaps/GM-Companion/wiki/changelog", "https://gm-companion.github.io/about.html"]

        Repeater {
            model: left_column.button_names

            Button {
                text: modelData
                anchors.left: parent.left
                anchors.right: parent.right

                onClicked: Qt.openUrlExternally(left_column.button_urls[index])
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Version: " + update_manager.getCurrentVersion()
            color: color_scheme.textColor
        }
    }

    TextArea {
        anchors.left: left_column.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        textFormat: TextEdit.RichText
        text: settings_tool.getCreditsPage()
        color: color_scheme.textColor
        font.pointSize: 12

        readOnly: true

        onLinkActivated: Qt.openUrlExternally(link)
    }
}
