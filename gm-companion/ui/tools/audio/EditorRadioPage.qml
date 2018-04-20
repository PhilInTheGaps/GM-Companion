import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

Page {
    id: radio_page

    signal saveRadio
    signal changeIcon(string path)
    signal changeURL(string url)
    signal changeLocal(bool local)

    property string resourcesPath

    function setName(name) {
        name_text.text = name
    }

    function setLocal(local) {
        local_switch.checked = local
    }

    function setURL(url) {
        url_textfield.text = url
    }

    function setIcon(path) {
        icon_textfield.text = path
        icon_image.source = "file://" + resourcesPath + path
    }

    Column {
        anchors.fill: parent
        spacing: 5

        Text {
            id: name_text
            text: qsTr("Radio")
        }

        Button {
            text: qsTr("Save Radio")
            onClicked: saveRadio()
        }

        Switch {
            id: local_switch
            text: qsTr("Local Mode")
            checked: false

            ToolTip.text: qsTr("In local mode, a local playlist is read instead of a web URL.")
            ToolTip.visible: hovered
            hoverEnabled: true

            onCheckedChanged: {
                changeLocal(checked)

                if (checked) {
                    url_text.text = qsTr("Playlist")
                    url_textfield.placeholderText = qsTr("Radio Playlist")
                } else {
                    url_text.text = qsTr("URL")
                    url_textfield.placeholderText = qsTr("Radio URL")
                }
            }
        }

        Text {
            id: url_text
            text: qsTr("URL")
        }

        TextField {
            id: url_textfield
            width: parent.width
            selectByMouse: true
            placeholderText: qsTr("Radio URL")

            onTextChanged: changeURL(text)
        }

        Text {
            text: qsTr("Radio Icon")
        }

        TextField {
            id: icon_textfield
            width: parent.width
            selectByMouse: true
            placeholderText: qsTr("Icon Path")

            onTextChanged: {
                changeIcon(text)
                icon_image.source = "file://" + resourcesPath + text
            }
        }

        Image {
            id: icon_image
            width: parent.width / 4
            height: width
            source: "/icons/media/radio_image.png"
        }
    }
}