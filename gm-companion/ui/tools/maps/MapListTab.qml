import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4

Tab {
    id: tab
    title: category
    anchors.fill: parent
    property var list
    property var paths
    property var category
    signal clicked(string path)

    ScrollView {
        id: scroll_view
        flickableItem.interactive: true
        flickableItem.flickableDirection: "VerticalFlick"

        anchors.fill: parent

        Column {
            id: column
            spacing: 5
            padding: 5
            width: 175
        }

        Component.onCompleted: {
            for (var i = 0; i < list.length; i++) {
                var component = Qt.createComponent("MapListItem.qml")
                var button = component.createObject(column, {
                                                        name: list[i],
                                                        path: paths[i]
                                                    })

                button.clicked.connect(tab.clicked)
            }
        }
    }
}
