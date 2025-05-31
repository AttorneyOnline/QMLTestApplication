import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Fusion
import Attorney.Audio

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        Rectangle {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "lightgray"

            RowLayout {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 10

                Button {
                    id: pausebutton
                    text: "Pause"
                    onClicked: Audio.pauseChannel(0)
                }

                Button {
                    id: resumebutton
                    text: "Resume"
                    onClicked: Audio.resumeChannel(0)
                }
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            id: songs
            model: Audio.songs();
            delegate: Button {
                width: ListView.view.width
                text: modelData.split(".")[0]
                onClicked: Audio.setChannelSong(0, "music/" + modelData)
                contentItem: Label {
                    text: parent.text
                    verticalAlignment: Text.AlignLeft
                }
            }
        }
    }
}
