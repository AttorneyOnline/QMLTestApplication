import QtQuick
import QtQuick.Controls
import com.kdab.dockwidgets 2.0 as KDDW

ApplicationWindow {
    id: root_window
    visible: true
    height: 600
    width: 600

    KDDW.DockingArea {
        id: root_docking
        anchors.fill:parent
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
            addDockWidget(absolute_example, KDDW.KDDockWidgets.Location_OnBottom)
            addDockWidget(flow_example, KDDW.KDDockWidgets.Location_OnTop)
            addDockWidget(scrollflow_example, KDDW.KDDockWidgets.Location_OnBottom)
        }
    }
}
