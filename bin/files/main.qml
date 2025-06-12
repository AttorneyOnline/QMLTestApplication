import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import com.kdab.dockwidgets 2.0 as KDDW

ApplicationWindow {
    id: root_window
    visible: true
    height: 600
    width: 600

    menuBar: MenuBar {
        height: 200

        Menu {
            title: qsTr("&File")

            Action {
                text: qsTr("Save layout")
                onTriggered: {
                    savedialog.open()
                }
            }

            Action {
                text: qsTr("Restore layout")
                onTriggered: {
                    loaddialog.open()
                }
            }

            Action {
                text: qsTr("Quit")
                onTriggered: {
                    Qt.quit()
                }
            }

            Action {
                text: qsTr("Reload Application")
                onTriggered: {
                    absolute_example.destroy()
                }
            }
        }
    }

    KDDW.DockingArea {
        id: root_docking
        anchors.fill: parent
        uniqueName: "MainLayout-1"

        KDDW.DockWidget {
            id: absolute_example
            uniqueName: "X/Y Position Example"
            source: "./files/absolute.qml"
        }

        KDDW.DockWidget {
            id: flow_example
            uniqueName: "FlowLayout"
            source: "./files/flowlayout.qml"
        }

        KDDW.DockWidget {
            id: scrollflow_example
            uniqueName: "Scrollflow"
            source: "./files/scrollflow.qml"
        }

        Component.onCompleted: {
            addDockWidget(absolute_example,
                          KDDW.KDDockWidgets.Location_OnBottom)
            addDockWidget(flow_example, KDDW.KDDockWidgets.Location_OnTop)
            addDockWidget(scrollflow_example,
                          KDDW.KDDockWidgets.Location_OnBottom)
        }
    }

    KDDW.LayoutSaver {
        id: layoutSaver
    }

    FileDialog {
        id: savedialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Attorney Layouts (*.aol)"]
        defaultSuffix: "aol"

        onAccepted: {
            var str = selectedFile.toString().replace("file:///", "")
            layoutSaver.saveToFile(str)
        }
    }

    FileDialog {
        id: loaddialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Attorney Layouts (*.aol)"]
        defaultSuffix: "aol"

        onAccepted: {
            var str = selectedFile.toString().replace("file:///", "")
            layoutSaver.restoreFromFile(str)
        }
    }
}
