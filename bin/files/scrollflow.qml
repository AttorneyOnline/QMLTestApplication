import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    anchors.fill: parent

    ScrollView {
        id: scrollview
        anchors.fill : parent // Refers to root

        Flow {
            width: scrollview.availableWidth // Width is defined by scrollview
            anchors.margins: 4
            spacing: 10

            Text { text: "Text"; font.pixelSize: 40 }
            Text { text: "items"; font.pixelSize: 40 }
            Text { text: "flowing"; font.pixelSize: 40 }
            Text { text: "inside"; font.pixelSize: 40 }
            Text { text: "a"; font.pixelSize: 40 }
            Text { text: "Flow"; font.pixelSize: 40 }
            Text { text: "item"; font.pixelSize: 40 }
        }
    }
}
