import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import Attorney.Audio

ApplicationWindow {
    id: window
    visible: true
    width: 1100
    height: 800
    title: "Audio Player"

    Material.theme: Material.Dark
    Material.accent: Material.Cyan

    property int selectedChannel: 0

    menuBar: MenuBar {
           Menu {
               title: qsTr("&Audio")

               MenuItem {
                   text: "Enable FadeIn"
                   checkable: true
                   checked: Audio.fadeIn()  // Initial state
                   onTriggered: Audio.setFadeIn(checked)
               }

               MenuItem {
                   text: "Enable FadeOut"
                   checkable: true
                   checked: Audio.fadeOut()  // Initial state
                   onTriggered: Audio.setFadeOut(checked)
               }
           }
    }


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

        // Audio Controls Section with Channel Selection
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 160
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

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                // Channel Selection (Vertical) - Fixed width and clipping
                Rectangle {
                    Layout.preferredWidth: 110
                    Layout.fillHeight: true
                    color: "#2a2a2a"
                    radius: 10
                    border.color: "#444444"
                    border.width: 1

                    ColumnLayout { // This ColumnLayout directly manages the items below
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 4

                        // The Repeater is now directly inside this ColumnLayout
                        Repeater {
                            model: 4
                            delegate: Button {
                                // Use Layout properties to let ColumnLayout manage size
                                Layout.fillWidth: true
                                Layout.fillHeight: true // This will make buttons share available height

                                text: "Ch " + (index)
                                font.pixelSize: 10
                                font.bold: true
                                checkable: true
                                checked: selectedChannel === index

                                onClicked: selectedChannel = index

                                background: Rectangle {
                                    anchors.fill: parent
                                    color: {
                                        if (parent.checked) return "#00bcd4"
                                        if (parent.pressed) return "#404040"
                                        if (parent.hovered) return "#333333"
                                        return "#1a1a1a"
                                    }
                                    radius: 8
                                    border.color: parent.checked ? "#ffffff" : "transparent"
                                    border.width: parent.checked ? 1 : 0

                                    Behavior on color {
                                        ColorAnimation { duration: 150 }
                                    }
                                }

                                contentItem: Item { // Keep the Item wrapper for robust centering
                                    anchors.fill: parent
                                    Text {
                                        anchors.centerIn: parent // Center the text within the Item
                                        text: parent.parent.text // Reference the button's text property
                                        color: parent.parent.checked ? "#ffffff" : "#cccccc"
                                        font: parent.parent.font
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }
                    }
                }


                // Main Audio Controls - Better proportions
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 15

                    // Playback Controls
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        spacing: 15

                        Button {
                            id: stopButton
                            text: "Stop Channel " + (selectedChannel)
                            font.pixelSize: 12
                            font.bold: true
                            Material.background: "#f44336"
                            Material.foreground: "#ffffff"
                            implicitHeight: 35
                            Layout.fillWidth: true
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 180

                            onClicked: Audio.stop(selectedChannel)

                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                color: parent.pressed ? "#c62828" : (parent.hovered ? "#d32f2f" : "#f44336")
                                radius: 18

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }
                        }

                        Button {
                            id: pauseButton
                            text: "Pause Channel " + (selectedChannel)
                            font.pixelSize: 13
                            font.bold: true
                            Material.background: "#ff5722"
                            Material.foreground: "#ffffff"
                            implicitHeight: 35
                            Layout.fillWidth: true
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 180

                            onClicked: Audio.pause(selectedChannel)

                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                color: parent.pressed ? "#d84315" : (parent.hovered ? "#e64a19" : "#ff5722")
                                radius: 18

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }
                        }

                        Button {
                            id: resumeButton
                            text: "Resume Channel " + (selectedChannel)
                            font.pixelSize: 13
                            font.bold: true
                            Material.background: "#4caf50"
                            Material.foreground: "#ffffff"
                            implicitHeight: 35
                            Layout.fillWidth: true
                            Layout.minimumWidth: 120
                            Layout.maximumWidth: 180

                            onClicked: Audio.resume(selectedChannel)

                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: "#ffffff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }

                            background: Rectangle {
                                color: parent.pressed ? "#2e7d32" : (parent.hovered ? "#388e3c" : "#4caf50")
                                radius: 18

                                Behavior on color {
                                    ColorAnimation { duration: 150 }
                                }
                            }
                        }
                    }

                    // Volume Controls for all 4 channels - More compact layout
                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        contentWidth: availableWidth
                        clip: true

                        ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                        ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                        GridLayout {
                            id: volumeGrid
                            width: parent.parent.width
                            columns: 4
                            rowSpacing: 8
                            columnSpacing: 10

                            Repeater {
                                model: 4
                                delegate: ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 6

                                    Text {
                                        text: "🔊 Ch" + (index)
                                        font.pixelSize: 12
                                        font.bold: true
                                        color: selectedChannel === index ? "#00bcd4" : "#ffffff"
                                        Layout.alignment: Qt.AlignHCenter

                                        Behavior on color {
                                            ColorAnimation { duration: 200 }
                                        }
                                    }

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 8

                                        Text {
                                            text: "Vol"
                                            font.pixelSize: 10
                                            color: "#ffffff"
                                        }

                                        Slider {
                                            id: channelVolumeSlider
                                            Layout.fillWidth: true
                                            Layout.minimumWidth: 80
                                            from: 0
                                            to: 100
                                            value: Audio.volume(index)

                                            Material.accent: selectedChannel === index ? "#00bcd4" : "#666666"

                                            property bool initialized: false
                                            property int channelIndex: index

                                            Component.onCompleted: {
                                                initialized = true
                                            }

                                            onValueChanged: {
                                                if (initialized) {
                                                    Audio.setVolume(channelIndex, value)
                                                }
                                            }

                                            background: Rectangle {
                                                x: channelVolumeSlider.leftPadding
                                                y: channelVolumeSlider.topPadding + channelVolumeSlider.availableHeight / 2 - height / 2
                                                implicitWidth: 80
                                                implicitHeight: 4
                                                width: channelVolumeSlider.availableWidth
                                                height: implicitHeight
                                                radius: 2
                                                color: "#404040"

                                                Rectangle {
                                                    width: channelVolumeSlider.visualPosition * parent.width
                                                    height: parent.height
                                                    color: selectedChannel === index ? "#00bcd4" : "#666666"
                                                    radius: 2

                                                    Behavior on color {
                                                        ColorAnimation { duration: 200 }
                                                    }
                                                }
                                            }

                                            handle: Rectangle {
                                                x: channelVolumeSlider.leftPadding + channelVolumeSlider.visualPosition * (channelVolumeSlider.availableWidth - width)
                                                y: channelVolumeSlider.topPadding + channelVolumeSlider.availableHeight / 2 - height / 2
                                                implicitWidth: 14
                                                implicitHeight: 14
                                                radius: 7
                                                color: channelVolumeSlider.pressed ? (selectedChannel === index ? "#0097a7" : "#555555") : (selectedChannel === index ? "#00bcd4" : "#666666")
                                                border.color: "#ffffff"
                                                border.width: 1

                                                Behavior on color {
                                                    ColorAnimation { duration: 150 }
                                                }
                                            }
                                        }

                                        Rectangle {
                                            Layout.preferredWidth: 35
                                            Layout.preferredHeight: 20
                                            color: "#333333"
                                            radius: 10
                                            border.color: selectedChannel === index ? "#00bcd4" : "#555555"
                                            border.width: 1

                                            Behavior on border.color {
                                                ColorAnimation { duration: 200 }
                                            }

                                            Text {
                                                anchors.centerIn: parent
                                                text: Math.round(channelVolumeSlider.value) + "%"
                                                font.family: "monospace"
                                                font.bold: true
                                                color: selectedChannel === index ? "#00bcd4" : "#cccccc"
                                                font.pixelSize: 8

                                                Behavior on color {
                                                    ColorAnimation { duration: 200 }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Audio Device Configuration Section
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 220
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
                opacity: 0.2
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: "🎧 Audio Device Configuration"
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

                    Text {
                        text: "Configure output devices per channel"
                        font.pixelSize: 12
                        color: "#888888"
                        font.italic: true
                    }
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    contentWidth: availableWidth
                    clip: true

                    ScrollBar.horizontal.policy: ScrollBar.AsNeeded
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                    GridLayout {
                        id: deviceGrid
                        width: parent.parent.width
                        columns: 2
                        rowSpacing: 10
                        columnSpacing: 15

                        Repeater {
                            model: 4
                            delegate: Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 70
                                color: selectedChannel === index ? "#2a2a3e" : "#2a2a2a"
                                radius: 10
                                border.color: selectedChannel === index ? "#00bcd4" : "#444444"
                                border.width: selectedChannel === index ? 2 : 1

                                // Store the channel index as a property
                                property int channelIndex: index

                                Behavior on color {
                                    ColorAnimation { duration: 200 }
                                }

                                Behavior on border.color {
                                    ColorAnimation { duration: 200 }
                                }

                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: 10
                                    spacing: 8

                                    RowLayout {
                                        Layout.fillWidth: true

                                        Rectangle {
                                            Layout.preferredWidth: 25
                                            Layout.preferredHeight: 25
                                            radius: 12.5
                                            color: selectedChannel === parent.parent.channelIndex ? "#00bcd4" : "#666666"

                                            Behavior on color {
                                                ColorAnimation { duration: 200 }
                                            }

                                            Text {
                                                anchors.centerIn: parent
                                                text: channelIndex
                                                color: "#ffffff"
                                                font.pixelSize: 12
                                                font.bold: true
                                            }
                                        }

                                        Text {
                                            text: "Channel " + (parent.parent.parent.channelIndex)
                                            font.pixelSize: 14
                                            font.bold: true
                                            color: selectedChannel === parent.parent.parent.channelIndex ? "#00bcd4" : "#ffffff"

                                            Behavior on color {
                                                ColorAnimation { duration: 200 }
                                            }
                                        }

                                        Rectangle {
                                            Layout.fillWidth: true
                                            height: 1
                                            color: "#444444"
                                        }

                                        Button {
                                            implicitWidth: 30
                                            implicitHeight: 25
                                            font.pixelSize: 12

                                            onClicked: selectedChannel = parent.parent.parent.channelIndex

                                            background: Rectangle {
                                                color: selectedChannel === parent.parent.parent.parent.channelIndex ? "#00bcd4" : (parent.hovered ? "#555555" : "transparent")
                                                radius: 5
                                                border.color: selectedChannel === parent.parent.parent.parent.channelIndex ? "#ffffff" : "#666666"
                                                border.width: 1

                                                Behavior on color {
                                                    ColorAnimation { duration: 150 }
                                                }
                                            }

                                            contentItem: Text {
                                                text: parent.text
                                                color: selectedChannel === parent.parent.parent.parent.parent.channelIndex ? "#ffffff" : "#cccccc"
                                                font: parent.font
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 10

                                        Text {
                                            text: "🔌"
                                            font.pixelSize: 14
                                            color: "#888888"
                                        }

                                        ComboBox {
                                            id: deviceComboBox
                                            Layout.fillWidth: true
                                            Layout.preferredHeight: 30

                                            property int channelIndex: parent.parent.parent.channelIndex
                                            property bool initialized: false

                                            model: Audio.availableDevices()

                                            // Function to update the current index based on the Audio backend
                                            function updateCurrentDevice() {
                                                if (!initialized) return
                                                var currentDevice = Audio.device(channelIndex)
                                                if (!currentDevice || currentDevice === "") {
                                                    return
                                                }

                                                // Find the index of the current device in the model
                                                var deviceIndex = -1
                                                for (var i = 0; i < model.length; i++) {
                                                    if (model[i] === currentDevice) {
                                                        deviceIndex = i
                                                        break
                                                    }
                                                }

                                                if (deviceIndex !== -1 && deviceIndex !== currentIndex) {
                                                    currentIndex = deviceIndex
                                                }
                                            }

                                            Component.onCompleted: {
                                                initialized = true
                                                updateCurrentDevice()
                                            }

                                            // Watch for changes in the selected channel to refresh device info
                                            Connections {
                                                target: window
                                                function onSelectedChannelChanged() {
                                                    if (channelIndex === selectedChannel) {
                                                        deviceComboBox.updateCurrentDevice()
                                                    }
                                                }
                                            }

                                            // Use onCurrentIndexChanged instead of onActivated for more reliable behavior
                                            onCurrentIndexChanged: {
                                                if (initialized && currentIndex >= 0 && currentIndex < model.length) {
                                                    var selectedDevice = model[currentIndex]
                                                    Audio.setDevice(channelIndex, selectedDevice)
                                                }
                                            }

                                            // Also keep onActivated as backup
                                            onActivated: function(index) {
                                                if (initialized && index >= 0 && index < model.length) {
                                                    var selectedDevice = model[index]
                                                    Audio.setDevice(channelIndex, selectedDevice)
                                                }
                                            }

                                            background: Rectangle {
                                                color: selectedChannel === parent.parent.parent.parent.channelIndex ? "#333344" : "#333333"
                                                radius: 8
                                                border.color: {
                                                    if (parent.activeFocus) return selectedChannel === parent.parent.parent.parent.channelIndex ? "#00bcd4" : "#666666"
                                                    if (parent.hovered) return selectedChannel === parent.parent.parent.parent.channelIndex ? "#0097a7" : "#555555"
                                                    return selectedChannel === parent.parent.parent.parent.channelIndex ? "#00bcd4" : "#444444"
                                                }
                                                border.width: 1

                                                Behavior on color {
                                                    ColorAnimation { duration: 150 }
                                                }

                                                Behavior on border.color {
                                                    ColorAnimation { duration: 150 }
                                                }
                                            }

                                            contentItem: Text {
                                                text: parent.displayText
                                                font.pixelSize: 11
                                                color: selectedChannel === parent.parent.parent.parent.parent.channelIndex ? "#ffffff" : "#cccccc"
                                                verticalAlignment: Text.AlignVCenter
                                                elide: Text.ElideRight
                                                leftPadding: 10
                                                rightPadding: 30

                                                Behavior on color {
                                                    ColorAnimation { duration: 200 }
                                                }
                                            }

                                            indicator: Text {
                                                x: parent.width - width - 8
                                                y: parent.height / 2 - height / 2
                                                text: "▼"
                                                font.pixelSize: 10
                                                color: selectedChannel === parent.parent.parent.parent.parent.channelIndex ? "#00bcd4" : "#888888"

                                                Behavior on color {
                                                    ColorAnimation { duration: 200 }
                                                }
                                            }

                                            delegate: ItemDelegate {
                                                id: comboDelegate
                                                width: deviceComboBox.width
                                                height: 35

                                                // Store the ComboBox channel index in the delegate
                                                property int delegateChannelIndex: deviceComboBox.channelIndex

                                                // Make sure the delegate is properly configured
                                                highlighted: deviceComboBox.highlightedIndex === index

                                                contentItem: Text {
                                                    text: modelData
                                                    font.pixelSize: 11
                                                    color: parent.highlighted ? "#ffffff" : (parent.hovered ? "#ffffff" : "#cccccc")
                                                    verticalAlignment: Text.AlignVCenter
                                                    elide: Text.ElideRight
                                                    leftPadding: 10

                                                    Behavior on color {
                                                        ColorAnimation { duration: 150 }
                                                    }
                                                }

                                                background: Rectangle {
                                                    color: parent.highlighted ? "#444444" : (parent.hovered ? "#333333" : "#2a2a2a")
                                                    radius: 4

                                                    Behavior on color {
                                                        ColorAnimation { duration: 150 }
                                                    }
                                                }

                                                // Ensure the delegate can be clicked
                                                MouseArea {
                                                    anchors.fill: parent
                                                    onClicked: {
                                                        deviceComboBox.currentIndex = index
                                                        deviceComboBox.popup.close()
                                                    }
                                                }
                                            }

                                            popup: Popup {
                                                y: parent.height + 2
                                                width: parent.width
                                                height: Math.min(contentItem.implicitHeight + 20, 200)
                                                padding: 10

                                                background: Rectangle {
                                                    color: "#2a2a2a"
                                                    radius: 8
                                                    border.color: "#444444"
                                                    border.width: 1
                                                }

                                                contentItem: ListView {
                                                    clip: true
                                                    implicitHeight: contentHeight
                                                    model: deviceComboBox.model
                                                    currentIndex: deviceComboBox.highlightedIndex

                                                    delegate: deviceComboBox.delegate

                                                    // Add explicit scrollbar
                                                    ScrollIndicator.vertical: ScrollIndicator { }
                                                }
                                            }
                                        }

                                        Text {
                                            Layout.preferredWidth: 80
                                            text: {
                                                var currentDevice = Audio.device(parent.parent.parent.channelIndex)
                                                return currentDevice.length > 12 ? currentDevice.substring(0, 12) + "..." : currentDevice
                                            }
                                            font.pixelSize: 9
                                            font.family: "monospace"
                                            color: selectedChannel === parent.parent.parent.channelIndex ? "#00bcd4" : "#888888"
                                            horizontalAlignment: Text.AlignRight
                                            elide: Text.ElideRight

                                            Behavior on color {
                                                ColorAnimation { duration: 200 }
                                            }

                                            ToolTip.visible: ma.containsMouse
                                            ToolTip.text: Audio.device(parent.parent.parent.channelIndex)
                                            ToolTip.delay: 500

                                            MouseArea {
                                                id: ma
                                                anchors.fill: parent
                                                hoverEnabled: true
                                            }
                                        }
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: selectedChannel = parent.channelIndex
                                    z: -1 // Behind other interactive elements
                                }
                            }
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

                    Text {
                        text: "Playing on: Channel " + (selectedChannel)
                        font.pixelSize: 12
                        color: "#00bcd4"
                        font.bold: true
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
                            verticalAlignment: Text.AlignVCenter

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
                            var allSongs = Audio.availableSongs()
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

                            onDoubleClicked: Audio.setSong(selectedChannel, "music/" + modelData)

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
                                    text: "→ Ch" + (selectedChannel)
                                    color: "#00bcd4"
                                    font.pixelSize: 10
                                    font.bold: true
                                    visible: parent.parent.hovered
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
