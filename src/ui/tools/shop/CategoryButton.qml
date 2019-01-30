import QtQuick 2.9
import QtQuick.Controls 2.2

import "../../components"

CustomButton {
    property string category: ""

    width: category_scroll_view.width
    height: color_scheme.toolbarHeight - 10

    buttonText: category
}
