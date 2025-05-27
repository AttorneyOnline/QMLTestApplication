import QtQuick

Rectangle {
   id: root
   anchors.fill: parent
   clip:true // This has a performance impact. Only clip where necessary.

   Rectangle {
       height: 100
       width: 100
       // z:0 // By default all sibling items are on z-level 0 unless defined otherwise
       color: "blue"
    }

   Rectangle {
       x: 50
       y:50
       z: 2  // Higher z value puts yellow on top
       height: 100
       width: 100
       color: "yellow"
   }

   Rectangle {
       x:25
       y:25
       z: 1  // Lower z value puts red behind yellow
       height: 100
       width: 100
       color: "red"
   }
}
