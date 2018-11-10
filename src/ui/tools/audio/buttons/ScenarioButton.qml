import QtQuick 2.9
import QtQuick.Controls 2.2
import gm.companion.platforms 1.0
import "../../../components"

CustomButton {
    width: text.width > 100 ? text.width + 10 : 100
    height: platform.isAndroid ? (parent ? parent.parent.height / 15 : 0) : 40

    text.anchors.horizontalCenter: horizontalCenter

    PlatformDetails {
        id: platform
    }
}
