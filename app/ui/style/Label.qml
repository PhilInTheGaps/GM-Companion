import QtQuick
import QtQuick.Templates as T
import "../colors.js" as Colors

T.Label {
    color: enabled ? Colors.text : Colors.textDisabled
}
