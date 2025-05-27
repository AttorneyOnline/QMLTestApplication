import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Attorney.Audio

Item {
    id: root
    anchors.fill: parent

    // Properties for managing state
    property string searchText: searchInput.text.toLowerCase()
    property int selectedChannel: 0
    property var filteredMusicList: []

    // Function to get display name from full path
    function getDisplayName(fullPath) {
        if (!fullPath) return ""

        // Get filename from path
        var fileName = fullPath.split('/').pop().split('\\').pop()

        // Remove file extension
        var lastDotIndex = fileName.lastIndexOf('.')
        if (lastDotIndex > 0) {
            fileName = fileName.substring(0, lastDotIndex)
        }

        // Replace underscores and dashes with spaces, capitalize
        fileName = fileName.replace(/[_-]/g, ' ')

        return fileName
    }

    // Function to filter music list based on search text
    function updateFilteredList() {
        var filtered = []
        if (Audio.musiclist) {
            for (var i = 0; i < Audio.musiclist.length; i++) {
                var song = Audio.musiclist[i]
                var displayName = getDisplayName(song)
                if (searchText === "" || displayName.toLowerCase().indexOf(searchText) !== -1) {
                    filtered.push({fullPath: song, displayName: displayName, originalIndex: i})
                }
            }
        }
        filteredMusicList = filtered
        musicListView.model = filteredMusicList
    }

    // Update filtered list when search text changes
    onSearchTextChanged: updateFilteredList()

    // Update filtered list when component completes
    Component.onCompleted: updateFilteredList()

    ScrollView {
        id: scrollView
        anchors.fill: parent

        ColumnLayout {
            width: scrollView.availableWidth
            spacing: 10

            // Channel status indicator and selector
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.margins: 10
                color: "#f8f8f8"
                border.color: "#d0d0d0"
                border.width: 1
                radius: 5

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        text: "Channels:"
                        font.pixelSize: 12
                        color: "#666666"
                    }

                    Row {
                        spacing: 8

                        Repeater {
                            model: 4

                            Rectangle {
                                width: 60
                                height: 20
                                color: selectedChannel === index ? "#4CAF50" : "#e0e0e0"
                                border.color: selectedChannel === index ? "#45a049" : "#bdbdbd"
                                border.width: 1
                                radius: 10

                                Text {
                                    anchors.centerIn: parent
                                    text: "CH" + (index + 1)
                                    font.pixelSize: 10
                                    color: selectedChannel === index ? "white" : "#666666"
                                    font.bold: selectedChannel === index
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor

                                    onClicked: {
                                        selectedChannel = index
                                        console.log("Selected channel:", index)
                                    }

                                    onEntered: parent.color = selectedChannel === index ? "#45a049" : "#d0d0d0"
                                    onExited: parent.color = selectedChannel === index ? "#4CAF50" : "#e0e0e0"
                                }
                            }
                        }
                    }

                    Item { Layout.fillWidth: true }

                    Text {
                        text: "Selected: CH" + (selectedChannel + 1) + " | Double-click songs to play"
                        font.pixelSize: 10
                        color: "#999999"
                        font.italic: true
                    }
                }
            }

            // Audio Controls
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.margins: 10
                color: "#f0f0f0"
                border.color: "#d0d0d0"
                border.width: 1
                radius: 5

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 10

                    Button {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 100
                        text: "Resume CH" + (selectedChannel + 1)
                        onPressed: Audio.resumeSong(selectedChannel)
                    }

                    Button {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 100
                        text: "Pause CH" + (selectedChannel + 1)
                        onPressed: Audio.pauseSong(selectedChannel)
                    }

                    Button {
                        Layout.fillWidth: true
                        Layout.preferredWidth: 100
                        text: "Stop CH" + (selectedChannel + 1)
                        onPressed: Audio.stopSong(selectedChannel)
                    }
                }
            }

            // Search bar
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.margins: 10
                color: "#f0f0f0"
                border.color: "#d0d0d0"
                border.width: 1
                radius: 5

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        text: "🔍"
                        font.pixelSize: 16
                        color: "#666666"
                    }

                    TextInput {
                        id: searchInput
                        Layout.fillWidth: true
                        font.pixelSize: 14
                        verticalAlignment: TextInput.AlignVCenter
                        selectByMouse: true
                        color: "#333333"

                        Text {
                            id: placeholder
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            text: "Search songs..."
                            color: "#999999"
                            font.pixelSize: 14
                            visible: searchInput.text.length === 0
                        }
                    }

                    // Clear search button
                    Rectangle {
                        width: 20
                        height: 20
                        color: "transparent"
                        visible: searchInput.text.length > 0

                        Text {
                            anchors.centerIn: parent
                            text: "×"
                            font.pixelSize: 16
                            color: "#666666"
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: searchInput.text = ""
                        }
                    }
                }
            }

            // Results counter
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 25
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                color: "transparent"
                visible: searchInput.text.length > 0

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: filteredMusicList.length + " song" + (filteredMusicList.length !== 1 ? "s" : "") + " found"
                    font.pixelSize: 12
                    color: "#666666"
                    font.italic: true
                }
            }

            // Music ListView
            ListView {
                id: musicListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredHeight: Math.max(contentHeight, 200)
                Layout.margins: 10
                clip: true

                model: filteredMusicList
                delegate: Rectangle {
                    width: musicListView.width
                    height: 60
                    color: mouseArea.containsMouse ? "#e6f3ff" : "transparent"
                    border.color: "#e0e0e0"
                    border.width: 1
                    radius: 3

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 15
                        spacing: 10

                        // Song icon
                        Text {
                            text: "♪"
                            font.pixelSize: 16
                            color: "#4CAF50"
                        }

                        // Song name
                        Text {
                            Layout.fillWidth: true
                            text: modelData.displayName || modelData
                            font.pixelSize: 14
                            color: "#333333"
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        // Channel indicator
                        Rectangle {
                            width: 40
                            height: 20
                            color: "#f0f0f0"
                            border.color: "#d0d0d0"
                            border.width: 1
                            radius: 10

                            Text {
                                anchors.centerIn: parent
                                text: "CH" + (selectedChannel + 1)
                                font.pixelSize: 9
                                color: "#666666"
                            }
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor

                        // Single click feedback
                        onClicked: {
                            console.log("Selected song:", modelData.displayName || modelData)
                        }

                        // Double-click to play on selected channel
                        onDoubleClicked: {
                            var songPath = modelData.fullPath || modelData
                            var displayName = modelData.displayName || modelData
                            console.log("Playing song:", displayName, "on channel", selectedChannel)
                            Audio.playSong(songPath, selectedChannel)

                            // Visual feedback for playing
                            parent.color = "#c8e6c9"
                            feedbackTimer.start()
                        }
                    }

                    // Timer for visual feedback
                    Timer {
                        id: feedbackTimer
                        interval: 500
                        onTriggered: parent.color = mouseArea.containsMouse ? "#e6f3ff" : "transparent"
                    }
                }

                // Empty state when no results
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width * 0.8
                    height: 120
                    color: "transparent"
                    visible: musicListView.count === 0

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 10

                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: "🎵"
                            font.pixelSize: 32
                            color: "#cccccc"
                        }

                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: searchInput.text.length > 0 ?
                                  "No songs found matching \"" + searchInput.text + "\"" :
                                  "No music available"
                            color: "#999999"
                            font.pixelSize: 14
                            horizontalAlignment: Text.AlignHCenter
                        }

                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: searchInput.text.length > 0 ? "Try a different search term" : ""
                            color: "#bbbbbb"
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            visible: searchInput.text.length > 0
                        }
                    }
                }
            }
        }
    }
}
