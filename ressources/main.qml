import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Universal
import Attorney.Audio
import Attorney.Settings

ApplicationWindow {
    visible:true

    Universal.theme: Universal.Light
    Universal.accent: Universal.Violet

    Flow {
        Button {
            id: playbutton
            text: "Play"
            onPressed: Audio.play()
        }

        Button {
            id: pausebutton
            text: "Pause"
            onPressed: Audio.pause()
        }

        Button {
            id: stopbutton
            text: "Stop"
            onPressed: Audio.stop()
        }

        Button {
            id: readsettingsbutton
            text: "Fetch from Settings"
            onPressed: console.log(Settings.readCustomValue("version/major"))
        }

        Button {
            id: writesettingsbutton
            text: "Fetch from Settings"
            onPressed: console.log(Settings.writeCustomValue("audio/somesecretsetting", "Spooky"))
        }
    }
}
