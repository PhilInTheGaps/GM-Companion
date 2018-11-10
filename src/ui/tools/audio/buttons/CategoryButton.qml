import QtQuick 2.9
import QtQuick.Controls 2.2
import gm.companion.platforms 1.0
import "../../../components"

CustomButton {
    width: parent ? parent.width : 0
    height: platform.isAndroid ? width / 6 : 40

    PlatformDetails {
        id: platform
    }
}
