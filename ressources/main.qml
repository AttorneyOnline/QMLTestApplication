import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Universal
import Attorney.Audio

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    Universal.theme: Universal.Light
    Universal.accent: Universal.Violet

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
            model: ["Cross-Examination - Allegro 2001 - AA.opus", "Athena Cykes - Courtroom Révolutionnaire 2016 - SOJ.opus", "Cross-Examination - Moderato 2001 - AA.opus", "https://hopedespairforce.com/base/sounds/music/%5baadd%5d%20athena%20cykes%20~%20courtroom%20revolutionnaire%20(lyrics).opus"]
            delegate: Button {
                width: ListView.view.width
                text: modelData
                onClicked: Audio.setChannelSong(0, modelData)
                contentItem: Label {
                    text: parent.text
                    verticalAlignment: Text.AlignLeft
                }
            }
        }
    }
}
