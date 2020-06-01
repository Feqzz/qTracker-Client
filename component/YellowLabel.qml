import QtQuick 2.0
import QtQuick.Templates 2.1 as T

T.Label {
    id: control
    color: ma.containsMouse ? "#d0d046" : "#ffffff"
    MouseArea {
        id: ma
        hoverEnabled: true
        anchors.fill: parent
    }
}
