import QtQuick.Controls
import src

Dialog {
    id: root

    signal toMarkerList

    title: qsTr("Delete?")
    modal: true

    standardButtons: Dialog.Yes | Dialog.No

    onAccepted: {
        MapTool.deleteMarker(MapTool.markerIndex);
        root.toMarkerList();
    }

    //    contentItem: Row {
    //        spacing: 10

    //        //        padding: 10

    //        Label {
    //            text: qsTr("Delete?")
    //            font.pointSize: 12
    //            anchors.verticalCenter: parent.verticalCenter
    //        }

    //        CustomToolBarButton {
    //            iconText: FontAwesome.check
    //            anchors.top: undefined
    //            anchors.bottom: undefined
    //            height: 30

    //            onClicked: {
    //                marker_delete_dialog.visible = false;
    //                MapTool.deleteMarker(MapTool.markerIndex);
    //                root.toMarkerList();
    //            }
    //        }

    //        CustomToolBarButton {
    //            iconText: FontAwesome.xmark
    //            anchors.top: undefined
    //            anchors.bottom: undefined
    //            height: 30

    //            onClicked: marker_delete_dialog.visible = false
    //        }
    //    }
}
