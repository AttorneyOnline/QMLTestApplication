import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import Attorney.Audio

ApplicationWindow {
    id: window
    visible: true
    width: 900
    height: 700
    title: "Audio Player"

    Material.theme: Material.Dark
    Material.accent: Material.Cyan

    // Gradient background
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1a1a2e" }
            GradientStop { position: 0.5; color: "#16213e" }
            GradientStop { position: 1.0; color: "#0f3460" }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Header
        Text {
            text: "🎵 Audio Player"
            font.pixelSize: 28
            font.bold: true
            color: "#ffffff"
            Layout.alignment: Qt.AlignHCenter

            // Simple text shadow effect using layered text
            Text {
                anchors.fill: parent
                text: parent.text
                font: parent.font
                color: "#000000"
                opacity: 0.3
                z: parent.z - 1
                anchors.topMargin: 2
                anchors.leftMargin: 2
            }
        }

        // Audio Controls Section
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            color: "#1e1e1e"
            radius: 15
            border.color: "#333333"
            border.width: 1

            // Subtle glow effect
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                radius: parent.radius
                border.color: "#00bcd4"
                border.width: 1
                opacity: 0.3
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                // Playback Controls
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 15

                    Button {
                        id: pauseButton
                        text: "⏸️ Pause"
                        font.pixelSize: 14
                        font.bold: true
                        Material.background: "#ff5722"
                        Material.foreground: "#ffffff"
                        implicitHeight: 40

                        onClicked: Audio.pauseChannel(0)

                        background: Rectangle {
                            color: parent.pressed ? "#d84315" : (parent.hovered ? "#e64a19" : "#ff5722")
                            radius: 20

                            Behavior on color {
                                ColorAnimation { duration: 150 }
                            }
                        }
                    }

                    Button {
                        id: resumeButton
                        text: "▶️ Resume"
                        font.pixelSize: 14
                        font.bold: true
                        Material.background: "#4caf50"
                        Material.foreground: "#ffffff"
                        implicitHeight: 40

                        onClicked: Audio.resumeChannel(0)

                        background: Rectangle {
                            color: parent.pressed ? "#2e7d32" : (parent.hovered ? "#388e3c" : "#4caf50")
                            radius: 20

                            Behavior on color {
                                ColorAnimation { duration: 150 }
                            }
                        }
                    }
                }

                // Volume Control
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 15

                    Text {
                        text: "🔊"
                        font.pixelSize: 20
                        color: "#ffffff"
                    }

                    Text {
                        text: "Volume"
                        font.pixelSize: 14
                        font.bold: true
                        color: "#ffffff"
                        Layout.preferredWidth: 60
                    }

                    Slider {
                        id: channel0Volume
                        Layout.fillWidth: true
                        Layout.maximumWidth: 300
                        from: 0
                        to: 100
                        value: Audio.volume(0)

                        Material.accent: "#00bcd4"

                        property bool initialized: false

                        Component.onCompleted: {
                            initialized = true
                        }

                        onValueChanged: {
                            if (initialized) {
                                Audio.setChannelVolume(0, channel0Volume.value)
                            }
                        }

                        background: Rectangle {
                            x: channel0Volume.leftPadding
                            y: channel0Volume.topPadding + channel0Volume.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 6
                            width: channel0Volume.availableWidth
                            height: implicitHeight
                            radius: 3
                            color: "#404040"

                            Rectangle {
                                width: channel0Volume.visualPosition * parent.width
                                height: parent.height
                                color: "#00bcd4"
                                radius: 3
                            }
                        }

                        handle: Rectangle {
                            x: channel0Volume.leftPadding + channel0Volume.visualPosition * (channel0Volume.availableWidth - width)
                            y: channel0Volume.topPadding + channel0Volume.availableHeight / 2 - height / 2
                            implicitWidth: 20
                            implicitHeight: 20
                            radius: 10
                            color: channel0Volume.pressed ? "#0097a7" : "#00bcd4"
                            border.color: "#ffffff"
                            border.width: 2

                            Behavior on color {
                                ColorAnimation { duration: 150 }
                            }
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 30
                        color: "#333333"
                        radius: 15
                        border.color: "#555555"
                        border.width: 1

                        Text {
                            anchors.centerIn: parent
                            text: Math.round(channel0Volume.value) + "%"
                            font.family: "monospace"
                            font.bold: true
                            color: "#00bcd4"
                            font.pixelSize: 12
                        }
                    }
                }
            }
        }

        // Song List Section
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1e1e1e"
            radius: 15
            border.color: "#333333"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "🎶 Song Library"
                        font.bold: true
                        font.pixelSize: 18
                        color: "#ffffff"
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 2
                        color: "#333333"
                        radius: 1
                    }
                }

                // Search Bar
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    color: "#2a2a2a"
                    radius: 20
                    border.color: searchField.activeFocus ? "#00bcd4" : "#555555"
                    border.width: 1

                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Text {
                            text: "🔍"
                            color: "#888888"
                            font.pixelSize: 16
                        }

                        TextField {
                            id: searchField
                            Layout.fillWidth: true
                            placeholderText: "Search songs..."
                            color: "#ffffff"
                            placeholderTextColor: "#888888"
                            font.pixelSize: 14

                            background: Rectangle {
                                color: "transparent"
                            }

                            selectByMouse: true
                        }

                        Button {
                            text: "✕"
                            visible: searchField.text.length > 0
                            implicitWidth: 25
                            implicitHeight: 25

                            onClicked: searchField.clear()

                            background: Rectangle {
                                color: parent.hovered ? "#ff5722" : "transparent"
                                radius: 12

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }

                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                font.pixelSize: 12
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded

                    ListView {
                        id: songsList
                        model: {
                            var allSongs = Audio.songs()
                            if (searchField.text.length === 0) {
                                return allSongs
                            }

                            var filteredSongs = []
                            var searchTerm = searchField.text.toLowerCase()

                            for (var i = 0; i < allSongs.length; i++) {
                                var songName = allSongs[i].split(".")[0].toLowerCase()
                                if (songName.includes(searchTerm)) {
                                    filteredSongs.push(allSongs[i])
                                }
                            }

                            return filteredSongs
                        }
                        spacing: 2

                        // Set explicit content size for horizontal scrolling
                        contentWidth: {
                            var maxWidth = 0
                            for (var i = 0; i < count; i++) {
                                var item = itemAtIndex(i)
                                if (item && item.implicitWidth > maxWidth) {
                                    maxWidth = item.implicitWidth
                                }
                            }
                            return Math.max(parent.width, maxWidth)
                        }

                        delegate: ItemDelegate {
                            height: 50
                            text: modelData.split(".")[0]

                            // Set width to accommodate text, but at least the viewport width
                            width: Math.max(songsList.parent.width, implicitWidth)

                            onDoubleClicked: Audio.setChannelSong(0, "music/" + modelData)

                            contentItem: RowLayout {
                                spacing: 15

                                Rectangle {
                                    Layout.preferredWidth: 35
                                    Layout.preferredHeight: 35
                                    radius: 17.5
                                    color: "#00bcd4"

                                    Text {
                                        anchors.centerIn: parent
                                        text: "♪"
                                        color: "#ffffff"
                                        font.pixelSize: 16
                                        font.bold: true
                                    }
                                }

                                Text {
                                    text: {
                                        var songName = parent.parent.text
                                        var searchTerm = searchField.text

                                        if (searchTerm.length > 0) {
                                            // Simple highlight effect - you could make this more sophisticated
                                            var lowerSong = songName.toLowerCase()
                                            var lowerSearch = searchTerm.toLowerCase()
                                            if (lowerSong.includes(lowerSearch)) {
                                                return songName // For now, just return the name - highlighting would need rich text
                                            }
                                        }
                                        return songName
                                    }
                                    Layout.fillWidth: true
                                    verticalAlignment: Text.AlignVCenter
                                    color: "#ffffff"
                                    font.pixelSize: 14
                                    elide: Text.ElideRight
                                }

                                Text {
                                    text: "Double-click to play"
                                    color: "#888888"
                                    font.pixelSize: 10
                                    visible: parent.parent.hovered

                                    Behavior on opacity {
                                        NumberAnimation { duration: 200 }
                                    }
                                }
                            }

                            background: Rectangle {
                                color: {
                                    if (parent.pressed) return "#333333"
                                    if (parent.hovered) return "#2a2a2a"
                                    return "transparent"
                                }
                                radius: 8
                                border.color: parent.activeFocus ? "#00bcd4" : "transparent"
                                border.width: 1

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
