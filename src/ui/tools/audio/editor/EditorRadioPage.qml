import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

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

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    Column {
        anchors.fill: parent
        spacing: 5

        Text {
            id: name_text
            text: qsTr("Radio")
            color: color_scheme.textColor
        }

        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 5

            Button {
                id: save_button
                text: qsTr("Save Radio")
                onClicked: saveRadio()
            }

            Switch {
                id: local_switch
                text: qsTr("Local Mode")
                checked: false

                width: parent.width - parent.spacing - save_button.width

                ToolTip.text: qsTr("In local mode, a local playlist is read instead of a web URL.")
                ToolTip.visible: hovered
                hoverEnabled: true

                background: Rectangle {
                    color: "#e0e0e0"
                }

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
        }

        Text {
            id: url_text
            text: qsTr("URL")
            color: color_scheme.textColor
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
            color: color_scheme.textColor
        }

        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 5

            TextField {
                id: icon_textfield
                width: parent.width - parent.spacing - icon_finder
                selectByMouse: true
                placeholderText: qsTr("Icon Path")

                onTextChanged: {
                    changeIcon(text)
                    icon_image.source = "file://" + resourcesPath + text
                }
            }

            IconFinder {
                id: icon_finder
                text_field: icon_textfield
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
