import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.4

import gm.companion.colorscheme 1.0
import gm.companion.platforms 1.0
import gm.companion.settingstool 1.0

Row {
    property string addon: "Placeholder Addon"
    property string description: "Placeholder Description, for what is probably a really awesome addon!"
    property string folder: "test"
    property bool addon_enabled: true

    anchors.left: parent ? parent.left : 0
    anchors.right: parent ? parent.right : 0

    padding: 5
    spacing: 5

    ColorScheme {
        id: color_scheme
    }

    PlatformDetails {
        id: platform
    }

    SettingsTool {
        id: settings
    }

    CheckBox {
        id: checkbox
        checked: addon_enabled
        padding: 0

        onClicked: {
            settings.setAddonEnabled(folder, checked)
        }
    }

    Text {
        id: name_text
        text: addon
        width: (parent.width - parent.spacing * 2 - parent.padding * 2) / 4
        clip: true
        elide: Text.ElideRight
        color: color_scheme.textColor
    }

    Rectangle {
        id: spacer
        width: 5
        height: parent.height - parent.padding * 2
        color: color_scheme.secondaryButtonColor
    }

    TextArea {
        width: parent.width - parent.spacing * 2 - parent.padding * 2
               - spacer.width - name_text.width
        text: description
        padding: 0

        readOnly: true
        clip: true
        wrapMode: "WordWrap"
    }
}
