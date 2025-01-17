﻿import QtQuick

Item {
        id: root 
        readonly property string version: "1.0"
        x: 0
        y: 0
        width: parent.width  
        height: parent.height
        property var control: parent
        MouseArea {
            anchors.fill: control
            onClicked: {
                parent.focus = true
        }
    }

    DragWidget {
        id: dragwidget
        mouseStyle: Qt.SizeAllCursor
        control: root.control
        anchors.fill: parent
        hoverenabled: false
        visible: control.focus
        onPosDragWidget: {
            control.x += pos.x
            control.y += pos.y
        }
    }

    DragSizeWidget {
        control: root.control
        anchors.fill: parent
        visible: control.focus
    }

}
