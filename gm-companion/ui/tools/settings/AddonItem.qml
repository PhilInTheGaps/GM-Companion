import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

import com.blackgrain.qml.quickdownload 1.0
import gm.companion.colorscheme 1.0

Row {
    property string addon: "Placeholder Addon"
    property string version: "1.0"
    property bool installed: false
    property bool outdated: false
    property string description: "Placeholder Description, for what is probably a really awesome addon! This description is really long and really informs everyone about the awesomeness of the addon..."
    property string destination

    width: parent ? parent.width : 0
    padding: 5
    spacing: 5

    ColorScheme {
        id: color_scheme
    }

    Column {
        id: left_column
        width: (parent.width - parent.spacing * 2 - parent.padding * 2) / 4
        spacing: 5

        Text {
            text: addon
            width: parent.width
            clip: true
            elide: Text.ElideRight
            color: color_scheme.textColor
        }

        Row {
            width: parent.width
            spacing: parent.spacing

            Text {
                id: version_indicator_text
                text: qsTr("Version:")
                anchors.verticalCenter: parent.verticalCenter
                color: color_scheme.textColor
            }

            Text {
                text: version
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width - parent.spacing - version_indicator_text.width
                clip: true
                elide: Text.ElideRight
                color: color_scheme.textColor
            }
        }

        Text {
            id: status_text
            text: installed ? (outdated ? qsTr("Outdated") : qsTr(
                                              "Installed")) : qsTr(
                                  "Not Installed")
            width: parent.width
            clip: true
            elide: Text.ElideRight
            color: color_scheme.textColor
        }

        Button {
            id: download_button
            text: qsTr("Download")
            enabled: !installed || outdated
            width: parent.width

            onClicked: {
                download.destination = "file://" + destination + "/" + addon
                        + "_" + version + ".zip"
                download.running = true
            }
        }
    }

    Rectangle {
        id: spacer
        width: 5
        height: parent.height - parent.padding * 2
        color: color_scheme.secondaryButtonColor
    }

    TextArea {
        width: parent.width - parent.spacing * 2 - parent.padding * 2
               - left_column.width - spacer.width
        height: parent.height - parent.padding * 2

        text: description
        readOnly: true
        clip: true
        wrapMode: "WordWrap"
    }

    Download {
        id: download

        url: "https://raw.githubusercontent.com/PhilInTheGaps/GM-Companion/master_release_1_0/gm-companion/Addons/"
             + addon + "_" + version + ".zip"

        running: false

        followRedirects: true
        onRedirected: console.log('Redirected', url, '->', redirectUrl)

        onStarted: {
            console.log('Started download', url)
            status_text.text = qsTr("Downloaded")
        }
        onError: {
            console.error(errorString)
            running = false
            status_text.text = qsTr("Download Failed")
        }

        onProgressChanged: console.log(url, 'progress:', progress)
        onFinished: {
            console.info(url, 'done')
            Qt.openUrlExternally(destination)
            status_text.text = qsTr("Downloaded")
            download_button.enabled = false
        }
    }
}
