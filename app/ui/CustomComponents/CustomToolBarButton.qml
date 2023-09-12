import QtQuick

CustomButton {
    id: root
    property bool verticalMode: false
    property bool outline: false

    anchors.top: verticalMode ? undefined : parent.top
    anchors.bottom: verticalMode ? undefined : parent.bottom
    anchors.left: verticalMode ? parent.left : undefined
    anchors.right: verticalMode ? parent.right : undefined
    anchors.margins: 5

    iconItem.style: outline ? Text.Outline : Text.Normal
    iconItem.styleColor: palette.dark
    enableBold: false
    pointSize: 14
    centering: verticalMode
    transparentBackground: true
}
