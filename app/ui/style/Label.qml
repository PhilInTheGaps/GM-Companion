import QtQuick 2.0
import QtQuick.Templates 2.2 as T
import "../colors.js" as Colors

T.Label {
    color: enabled ? Colors.text : Colors.textDisabled
}
