pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import CustomComponents

Dialog {
    id: root

    signal insert(string text)

    modal: true

    contentItem: Item {
        implicitWidth: 200
        implicitHeight: column.height

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
                model: 6

                CustomButton {
                    required property int modelData
                    property int headerNum: modelData + 1
                    property string openTag: "<h" + headerNum + ">"
                    property string closeTag: "</h" + headerNum + ">"

                    buttonText: openTag + qsTr(
                                    "Header") + " " + headerNum + closeTag
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: textItem.height

                    onClicked: {
                        root.accept()

                        var hashes = ""

                        for (var i = 0; i < modelData + 1; i++) {
                            hashes += "#"
                        }

                        root.insert(hashes + " ")
                    }
                }
            }
        }
    }
}
