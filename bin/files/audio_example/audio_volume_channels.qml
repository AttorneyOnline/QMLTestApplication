import QtQuick 2.15
import QtQuick.Controls.Material
import QtQuick.Layouts 1.15
import Attorney.Audio

Item {
    id: root
    anchors.fill: parent

    ScrollView {
        id: scrollView
        anchors.fill: parent

        Flow {
            id: flow
            width: scrollView.availableWidth
            spacing: 10

            // Channel 0
            Rectangle {
                width: 200
                height: layout0.implicitHeight + 20
                color: Material.backgroundColor
                border.color: Material.dividerColor
                border.width: 1
                radius: 4

                ColumnLayout {
                    id: layout0
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 10

                    Text {
                        text: "Channel 1"
                        Layout.alignment: Qt.AlignHCenter
                        font.bold: true
                    }

                    Slider {
                        Layout.fillWidth: true
                        Material.accent: Material.Orange
                        from: 0
                        to: 100
                        value: Audio.channel0Volume
                        onValueChanged: Audio.setVolume(value, 0)
                    }

                    ComboBox {
                        Layout.fillWidth: true
                        model: Audio.audioDevices
                        currentIndex: model ? model.indexOf(Audio.channel0Device) : -1
                        onActivated: function(index) {
                            console.log("Channel 0:", textAt(index))
                            Audio.setDevice(textAt(index), 0)
                        }
                    }
                }
            }

            // Channel 1
            Rectangle {
                width: 200
                height: layout1.implicitHeight + 20
                color: Material.backgroundColor
                border.color: Material.dividerColor
                border.width: 1
                radius: 4

                ColumnLayout {
                    id: layout1
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 10

                    Text {
                        text: "Channel 2"
                        Layout.alignment: Qt.AlignHCenter
                        font.bold: true
                    }

                    Slider {
                        Layout.fillWidth: true
                        Material.accent: Material.Orange
                        from: 0
                        to: 100
                        value: Audio.channel1Volume
                        onValueChanged: Audio.setVolume(value, 1)
                    }

                    ComboBox {
                        Layout.fillWidth: true
                        model: Audio.audioDevices
                        currentIndex: model ? model.indexOf(Audio.channel1Device) : -1
                        onActivated: function(index) {
                            console.log("Channel 1:", textAt(index))
                            Audio.setDevice(textAt(index), 1)
                        }
                    }
                }
            }

            // Channel 2
            Rectangle {
                width: 200
                height: layout2.implicitHeight + 20
                color: Material.backgroundColor
                border.color: Material.dividerColor
                border.width: 1
                radius: 4

                ColumnLayout {
                    id: layout2
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 10

                    Text {
                        text: "Channel 3"
                        Layout.alignment: Qt.AlignHCenter
                        font.bold: true
                    }

                    Slider {
                        Layout.fillWidth: true
                        Material.accent: Material.Orange
                        from: 0
                        to: 100
                        value: Audio.channel2Volume
                        onValueChanged: Audio.setVolume(value, 2)
                    }

                    ComboBox {
                        Layout.fillWidth: true
                        model: Audio.audioDevices
                        currentIndex: model ? model.indexOf(Audio.channel2Device) : -1
                        onActivated: function(index) {
                            console.log("Channel 2:", textAt(index))
                            Audio.setDevice(textAt(index), 2)
                        }
                    }
                }
            }

            // Channel 3
            Rectangle {
                width: 200
                height: layout3.implicitHeight + 20
                color: Material.backgroundColor
                border.color: Material.dividerColor
                border.width: 1
                radius: 4

                ColumnLayout {
                    id: layout3
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 10

                    Text {
                        text: "Channel 4"
                        Layout.alignment: Qt.AlignHCenter
                        font.bold: true
                    }

                    Slider {
                        Layout.fillWidth: true
                        Material.accent: Material.Orange
                        from: 0
                        to: 100
                        value: Audio.channel3Volume
                        onValueChanged: Audio.setVolume(value, 3)
                    }

                    ComboBox {
                        Layout.fillWidth: true
                        model: Audio.audioDevices
                        currentIndex: model ? model.indexOf(Audio.channel3Device) : -1
                        onActivated: function(index) {
                            console.log("Channel 3:", textAt(index))
                            Audio.setDevice(textAt(index), 3)
                        }
                    }
                }
            }
        }
    }
}
