import QtQuick

Rectangle {
    id: root

    Text {
        anchors.centerIn: parent
        id: message
        text: qsTr("To load more docks. Please use File > Load File in the menubar")
        font.pointSize: 15

        width: parent.width
        wrapMode: Text.WordWrap
    }
}
