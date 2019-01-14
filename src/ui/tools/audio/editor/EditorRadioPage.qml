import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

Page {
    id: radio_page

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

    background: Rectangle {
        color: color_scheme.backgroundColor
    }

    TextField {
        id: url_textfield
        anchors.left: parent.left
        anchors.right: local_switch.left
        anchors.top: parent.top
        height: color_scheme.toolbarHeight

        selectByMouse: true
        placeholderText: audio_editor.local ? qsTr("Radio Playlist") : qsTr(
                                                  "Radio URL")
        text: audio_editor.url

        onTextEdited: audio_editor.setUrl(audio_editor.name,
                                          audio_editor.type, text)
    }

    Switch {
        id: local_switch
        text: qsTr("Local Mode")
        checked: audio_editor.local

        anchors.top: parent.top
        anchors.right: radio_file_browser.left
        height: color_scheme.toolbarHeight

        ToolTip.text: qsTr("In local mode, a local playlist is read instead of a web URL.")
        ToolTip.visible: hovered
        hoverEnabled: true

        onClicked: {
            audio_editor.setLocal(audio_editor.name, audio_editor.type, checked)
        }
    }

    EditorFileBrowser {
        id: radio_file_browser
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: right_item.left
        anchors.rightMargin: 5
        width: 175
    }

    Rectangle {
        id: right_item

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        width: color_scheme.toolbarHeight
        color: color_scheme.menuColor
    }
}
