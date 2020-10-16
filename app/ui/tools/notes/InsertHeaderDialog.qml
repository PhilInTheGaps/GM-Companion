import QtQuick 2.6
import QtQuick.Controls 2.2
import CustomComponents 1.0
import "../../defines.js" as Defines

Dialog {
    id: root

    signal insert(string text)

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

                        insert(hashes + " ")
                    }
                }
            }
        }
    }
}
