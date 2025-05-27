import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import com.kdab.dockwidgets 2.0 as KDDW
import QtQuick.Layouts
import Attorney.Audio
import Attorney.Theme

// BE CAREFUL WHEN TOUCHING THIS FILE
// THIS IS SYSTEM CODE THAT SHOULD NOT BE EDITED UNLESS YOU KNOW WHAT YOU ARE DOING
ApplicationWindow {
    id: root
    visible: true
    height: 600
    width: 600

    property var docks: []

    menuBar: MenuBar {
        Menu {
            id: file_menu
            title: qsTr("&File")

            Action {
                text: qsTr("Load File")
                onTriggered: dock_loadDialog.open()
            }

            Action {
                text: qsTr("Quit")
                onTriggered: {
                    Qt.quit()
                }
            }
        }

        Menu {
            id: layouts_menu
            title: qsTr("&Layouts")

            Action {
                text: qsTr("Save layout")
                onTriggered: layout_savedialog.open()
            }

            Action {
                text: qsTr("Restore layout")
                onTriggered: layout_loaddialog.open()
            }
        }

        Menu {
            id: docks_menu
            title: qsTr("&Docks")

            MenuItem {
                text: qsTr("Reload All Docks")
                enabled: root.docks.length > 0
                onTriggered: {
                    reloadAllDocks()
                }
            }

            MenuItem {
                text: qsTr("Remove All Docks")
                enabled: root.docks.length > 0
                onTriggered: {
                    removeAllDocks()
                }
            }

            MenuSeparator {
                visible: root.docks.length > 0
            }

            Instantiator {
                model: root.docks

                Menu {
                    title: modelData.uniqueName

                    MenuItem {
                        text: qsTr("Visible")
                        checkable: true
                        checked: modelData.visible
                        onTriggered: toggleDockVisibility(modelData)
                    }

                    MenuItem {
                        text: qsTr("Reload")
                        onTriggered: {
                            reloadDock(modelData)
                        }
                    }

                    MenuItem {
                        text: qsTr("Close")
                        onTriggered: {
                            removeDock(modelData)
                        }
                    }
                }

                onObjectAdded: function (index, object) {
                    docks_menu.insertMenu(docks_menu.count, object)
                }

                onObjectRemoved: function (index, object) {
                    docks_menu.removeMenu(object)
                }
            }
        }

        Menu {
            id: audio_menu
            title: qsTr("&Audio")

            MenuItem {
                text: qsTr("Enable FadeIn")
                checkable: true
                checked: Audio.fadeIn
                onToggled: Audio.fadeIn = checked
            }

            MenuItem {
                text: qsTr("Enable FadeOut")
                checkable: true
                checked: Audio.fadeOut
                onToggled: Audio.fadeOut = checked
            }

            MenuItem {
                text: qsTr("Enable Synchronization")
                checkable: true
                checked: Audio.synchronize
                onToggled: Audio.synchronize = checked
            }
        }
    }

    KDDW.DockingArea {
        id: root_dock
        anchors.fill: parent
        uniqueName: "MainLayout-1"

        Component.onCompleted: {
            createDock("./files/system/message.qml")
        }
    }

    KDDW.LayoutSaver {
        id: layoutSaver
    }

    FileDialog {
        id: layout_savedialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Attorney Layouts (*.aol)"]
        defaultSuffix: "aol"

        onAccepted: {
            var str = selectedFile.toString().replace("file:///", "")
            layoutSaver.saveToFile(str)
        }
    }

    FileDialog {
        id: layout_loaddialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Attorney Layouts (*.aol)"]

        onAccepted: {
            var str = selectedFile.toString().replace("file:///", "")
        }
    }

    FileDialog {
        id: dock_loadDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Qt QML File (*.qml)"]

        onAccepted: {
            var str = selectedFile.toString().replace("file:///", "")
            createDock(str)
        }
    }

    function createDock(source) {
        console.log("Creating dock from " + source)
        var component = Qt.createComponent("./dynamicdock.qml")
        var uniqueName = getDisplayNameFromPath(source) + Date.now()
        var dock = component.createObject(root_dock, {
                                              "source": source,
                                              "uniqueName": uniqueName
                                          })
        root_dock.addDockWidget(dock, KDDW.KDDockWidgets.Location_OnBottom)
        addDockToMenu(dock)
    }

    function addDockToMenu(data) {
        root.docks.push(data)
        root.docks = root.docks // Force reload of model
        console.log(root.docks.length)
    }

    function reloadDock(data) {
        var uniqueName = data.uniqueName
        var source = data.source

        Theme.clearComponentCache()

        for (var index = 0; index < root.docks.length; index++) {
            if (root.docks[index] === data) {
                layoutSaver.saveToFile("autosave.aol")
                removeDock(data)
                console.log("Creating dock from " + source)
                var component = Qt.createComponent("./dynamicdock.qml")
                var dock = component.createObject(root_dock, {
                                                      "source": source,
                                                      "uniqueName": uniqueName
                                                  })
                root_dock.addDockWidget(dock,
                                        KDDW.KDDockWidgets.Location_OnBottom)
                root.docks[index] = dock
                root.docks = root.docks

                layoutSaver.restoreFromFile("autosave.aol")
            }
        }
    }

    function reloadAllDocks() {
        for (var index = 0; index < root.docks.length; index++) {
            reloadDock(root.docks[index])
        }
    }

    function removeDock(data) {
        for (var index = 0; index < root.docks.length; index++) {
            if (root.docks[index] === data) {
                console.log("Deleting Dock")
                root.docks[index].deleteDockWidget()
                root.docks.splice(index, 1)
                break
            }
        }
        root.docks = root.docks
    }

    function removeAllDocks() {
        for (var index = 0; index < root.docks.length; index++) {
            root.docks[index].deleteDockWidget()
        }
        root.docks = []
    }

    function getDisplayNameFromPath(filePath) {
        var fileName = filePath.split('/').pop().split('\\').pop()
        return fileName.replace('.qml', '')
    }

    function toggleDockVisibility(dock) {
        if (dock.visible) {
            // Apparently there is a isOpen() function that does not seem to actually exist :shrug:
            dock.close()
            dock.visible = false
        } else {
            dock.show()
        }
    }
}
